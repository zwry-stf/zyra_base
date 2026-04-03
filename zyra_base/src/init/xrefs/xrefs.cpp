#include <zyra/init/xrefs/xrefs.h>
#include <zyra/util/zyra_error.h>
#include <zyra/util/log.h>
#include <algorithm>
#include <Windows.h>


zyra_begin_

void* find_function_start(char* address, char* text_start) {
    while (address - 1 > text_start) {
        if (static_cast<std::uint8_t>(*--address) == 0xCCu &&
            static_cast<std::uint8_t>(*(address - 1)) == 0xCCu) {
            return address + 1;
        }
    }

    return nullptr;
}

bool get_section(const xstr& module, const basic_xstr<char, 8>& section_name,
                 void*& start, std::size_t& size) {
    char buffer[module.kMaxLength + 1];
    module.get(buffer);
    auto h = GetModuleHandleA(buffer);
    if (!h) {
        return false;
    }

    auto* dos = reinterpret_cast<PIMAGE_DOS_HEADER>(h);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE)
        return false;

    auto* nt = reinterpret_cast<PIMAGE_NT_HEADERS>(
        reinterpret_cast<BYTE*>(h) + dos->e_lfanew
    );

    if (nt->Signature != IMAGE_NT_SIGNATURE)
        return false;

    PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(nt);

    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; ++i) {
        char sec_name[9]{};
        std::memcpy(sec_name, section[i].Name, 8);

        if (section_name.equals(sec_name)) {
            start = reinterpret_cast<void*>(
                reinterpret_cast<BYTE*>(h) + section[i].VirtualAddress
            );

            size = static_cast<std::size_t>(section[i].Misc.VirtualSize);
            if (size == 0)
                size = static_cast<std::size_t>(section[i].SizeOfRawData);

            return true;
        }
    }

    return false;
}

std::vector<void*> find_xrefs(void* address, const char* start, const char* end)
{
    std::vector<void*> ret;
    assert(address != nullptr);
    assert(start < end);

    const std::uintptr_t target = reinterpret_cast<std::uintptr_t>(address);

    for (auto p = reinterpret_cast<const std::uint8_t*>(start);
         p < reinterpret_cast<const std::uint8_t*>(end) - 8; ++p) {
        if (std::memcmp(p, &target, sizeof(std::uintptr_t)) == 0) {
            ret.push_back(const_cast<void*>(static_cast<const void*>(p)));
            continue;
        }

        if ((p[0] & 0xF0) == 0x40 && 
            (p[1] == 0x8D || p[1] == 0x8B) && // lea or mov
            (p[2] & 0xC7) == 0x05) {
            const std::int32_t disp = *reinterpret_cast<const std::int32_t*>(p + 3);
            const std::uintptr_t calculated = reinterpret_cast<std::uintptr_t>(
                p + 3 + sizeof(std::int32_t)
            ) + static_cast<std::intptr_t>(disp);

            if (calculated == target) {
                ret.push_back(const_cast<void*>(static_cast<const void*>(p)));  // start of the instruction
            }
        }
    }

    return ret;
}

void* find_string(const basic_xstr<char, 128>& name, const char* start, const char* end) {
    assert(!name.empty());

    std::string null_terminated(name.length() + 1, '\0');
    name.get(null_terminated.data(), null_terminated.size());
    null_terminated.resize(name.length() + 1);
    auto it = std::search(start, end, null_terminated.begin(), null_terminated.end());
    if (it != end) {
        return const_cast<char*>(it);
    }
    return nullptr;
}

c_xrefs::c_xrefs() = default;

void* c_xrefs::get_function_by_xref(const xstr& module, const std::vector<basic_xstr<char, 128>>& strings)
{
    assert(!strings.empty());

    void* rdata_base;
    std::size_t rdata_size;
    if (!get_section(module, ".rdata", rdata_base, rdata_size)) {
        log::print_error("failed to get .rdata section for '{}'.", module);
        return nullptr;
    }

    std::vector<void*> string_addresses;
    string_addresses.reserve(strings.size());
    for (const auto& str : strings) {
        void* rdata_str = find_string(
            str,
            reinterpret_cast<char*>(rdata_base),
            reinterpret_cast<char*>(rdata_base) + rdata_size
        );
        if (rdata_str == nullptr) {
            log::print_error("failed to find xref string '{}'.", str);
            return nullptr;
        }
        string_addresses.push_back(rdata_str);
    }

    void* text_base;
    std::size_t text_size;
    if (!get_section(module, ".text", text_base, text_size)) {
        log::print_error("failed to get .text section for '{}'.", module);
        return nullptr;
    }

    std::vector<void*> found_functions;
    for (std::size_t i = 0; i < string_addresses.size(); i++) {
        const auto xrefs = find_xrefs(
            string_addresses[i],
            reinterpret_cast<char*>(text_base),
            reinterpret_cast<char*>(text_base) + text_size
        );
        if (xrefs.empty()) {
            log::print_error("no xrefs found for xref '{}'.", strings[i]);
            return nullptr;
        }

        std::vector<void*> new_vec;
        for (void* xref : xrefs) {
            void* fn = find_function_start(
                reinterpret_cast<char*>(xref),
                reinterpret_cast<char*>(text_base)
            );
            if (fn == nullptr) {
                log::print_error("failed to find function start for xref '{}'.", strings[i]);
                return nullptr;
            }

            if (found_functions.empty()) {
                new_vec.push_back(fn);
            }
            else {
                bool found = false;
                for (const auto& f : found_functions) {
                    if (f == fn) {
                        found = true;
                        break;
                    }
                }

                if (found) {
                    new_vec.push_back(fn);
                }
            }
        }

        if (new_vec.empty()) {
            log::print_error("no matching xrefs found for xrefs '{}'...", strings.front());
            return nullptr;
        }

        found_functions = std::move(new_vec);
    }

    if (found_functions.size() != 1) {
        log::print_error("more than one xref found for xrefs '{}'...", strings.front());
        return nullptr;
    }

    return found_functions[0];
}

void* c_xrefs::get_impl(const string_token& hash, bool do_try) const
{
    for (auto& sig : xrefs_)
        if (sig.hash == hash)
            return sig.address;

    if (do_try)
        return nullptr;

    log::print_error("failed to acquire vtable/signature/xref '{}'", hash);
    std::unreachable();
}

void c_xrefs::on_init()
{
#if defined(_DEBUG)
    if (failed_)
        throw zyra_error("failed to find xrefs");
#endif // _DEBUG
}

void c_xrefs::add(const string_token& name, const xstr& module, const std::vector<basic_xstr<char, 128>>& strings)
{
    void* res = get_function_by_xref(module, strings);
    if (res == nullptr) {
        log::print_error("failed to find xref '{}'", name);
#if defined(_DEBUG)
        failed_ = true;
        return;
#else
        throw zyra_error("failed to find xrefs");
#endif // _DEBUG
    }

    log::print_debug("xref '{}' found at {}", name, res);

    xrefs_.emplace_back(res, name);
}

zyra_end_
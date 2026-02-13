#include <zyra/init/signatures/signatures.h>
#include <zyra/util/zyra_error.h>
#include <zyra/util/log.h>
#include <zyra/init/vtables/vtables.h>
#include <Windows.h>


zyra_begin_

c_signatures::c_signatures()
#if defined(_DEBUG)
    : failed_(false)
#endif // _DEBUG
{
}


void c_signatures::on_init()
{
#if defined(_DEBUG)
    if (failed_)
        throw zyra_error("failed to find signatures");
#endif // _DEBUG
}

void* c_signatures::find_signature(void* start, void* end,
                                   const default_signature& signature, bool debug)
{
    std::uint8_t last_byte = static_cast<std::uint8_t>(signature[signature.length() - 1u]);

    std::uintptr_t range_end = reinterpret_cast<std::uintptr_t>(end) - signature.length();
    void* found = 0;
    for (std::uintptr_t address = reinterpret_cast<std::uintptr_t>(start) + signature.length() - 1u;
        address <= range_end; ) {
        void* hit = std::memchr(
            reinterpret_cast<void*>(address), last_byte, static_cast<std::size_t>(range_end - address + 1u));
        if (hit == nullptr)
            break;

        std::uintptr_t ptr = reinterpret_cast<std::uintptr_t>(hit) - signature.length() + 1u; // candidate start address

        bool match = true;
        for (std::uint32_t i = 0u; i < signature.length(); ++i) {
            if (signature[i] == -1) 
                continue;

            if (*reinterpret_cast<std::uint8_t*>(ptr + i) != 
                    static_cast<std::uint8_t>(signature[i])) {
                match = false;
                break;
            }
        }

        if (match) {
            if (found != nullptr && debug) {
                log::print_warning("multiple results found for signature");
            }

            // relative address
            if (signature.abs_offset() != 0u) {
                ptr += signature.abs_offset();
                ptr += sizeof(int32_t) + *reinterpret_cast<int32_t*>(ptr);
            }

            // Save
            if (!debug)
                return (void*)ptr;

            found = (void*)ptr;
        }

        address = reinterpret_cast<std::uintptr_t>(hit) + 1; // continue after this anchor hit
    }

    return found;
}

void* c_signatures::find_signature(const xstr& _m, const default_signature& signature)
{
    char module[xstr::kMaxLength];
    _m.get(module);

    std::uintptr_t mod = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(module));
    if (mod == 0)
        return nullptr;

    auto dos = reinterpret_cast<IMAGE_DOS_HEADER*>(mod);
    auto nt = reinterpret_cast<IMAGE_NT_HEADERS*>(mod + dos->e_lfanew);

    auto sec = IMAGE_FIRST_SECTION(nt);

    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; ++i) {
        const auto& s = sec[i];

        if ((s.Characteristics & IMAGE_SCN_MEM_EXECUTE) == 0)
            continue;

        std::uintptr_t base = mod + s.VirtualAddress;
        std::size_t len = s.Misc.VirtualSize ? s.Misc.VirtualSize : s.SizeOfRawData;
        void* res = find_signature(
            reinterpret_cast<void*>(base),
            reinterpret_cast<void*>(base + len),
            signature
#if defined(_DEBUG)
            , true
#endif // _DEBUG
        );
        if (res != nullptr)
            return res;
    }

    return nullptr;
}

void* c_signatures::get_impl(const string_token& hash, bool do_try) const
{
    for (auto& sig : signatures_)
        if (sig.hash == hash)
            return sig.address;

    return do_try ? 
        g_vtables()->try_get_function<void*>(hash) :
        g_vtables()->get_function<void*>(hash);
}

void c_signatures::add(const string_token& name, const xstr& module, const default_signature& signature)
{
#if defined(_DEBUG)
    for (auto& sig : signatures_)
        assert(sig.hash != name);
#endif

    void* ret = find_signature(module, signature);
    if (ret == nullptr) {
        log::print_error("failed to find signature '{}'", name);
#if defined(_DEBUG)
        failed_ = true;
        return;
#else
        throw zyra_error("failed to find signatures");
#endif // _DEBUG
    }

    log::print_debug("signature '{}' found at {}", name, ret);

    signatures_.emplace_back(ret, name);
}

zyra_end_
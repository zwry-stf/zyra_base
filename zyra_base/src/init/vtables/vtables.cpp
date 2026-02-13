#include <zyra/init/vtables/vtables.h>
#include <zyra/util/zyra_error.h>
#include <zyra/util/log.h>
#include <zyra/init/signatures/signatures.h>
#include <Windows.h>


zyra_begin_

c_vtables::c_vtables()
#ifdef _DEBUG
    : failed_(false)
#endif
{
}

void c_vtables::on_init()
{
#if defined(_DEBUG)
    if (failed_)
        throw zyra_error("failed to find vtables");
#endif
}

inline static bool get_section_info(std::uintptr_t base_address, const std::string& section_name,
                                    std::uintptr_t& section_start, std::uintptr_t& section_size)
{
    const PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)base_address;
    if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
        return false;

    const PIMAGE_NT_HEADERS32 nt_headers = (PIMAGE_NT_HEADERS32)(base_address + dos_header->e_lfanew);
    if (nt_headers->Signature != IMAGE_NT_SIGNATURE)
        return false;

    PIMAGE_SECTION_HEADER section_header = IMAGE_FIRST_SECTION(nt_headers);
    std::uint16_t number_of_sections = nt_headers->FileHeader.NumberOfSections;
    while (number_of_sections > 0) {
        // If we're at the right section
        if (!strcmp(section_name.c_str(), (const char*)section_header->Name)) {
            section_start = base_address + section_header->VirtualAddress;
            section_size = section_header->SizeOfRawData;
            return true;
        }

        section_header++;
        number_of_sections--;
    }

    return false;
}


void* c_vtables::find_vtable(const xstr& _m, const default_vtable& vtable)
{
    char module[xstr::kMaxLength];
    _m.get(module);

    std::uintptr_t base_address = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(module));
    if (!base_address) {
        log::print_error("failed to find module '{}'", module);
        return nullptr; 
    }

    auto DosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(base_address);
    auto NTHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(base_address + DosHeader->e_lfanew);
    const std::size_t base_size = static_cast<std::size_t>(NTHeaders->OptionalHeader.SizeOfImage);

    std::uintptr_t rtti_type_descriptor = reinterpret_cast<std::uintptr_t>(
        g_signatures()->find_signature((void*)base_address, (void*)(base_address + base_size), vtable));
    if (rtti_type_descriptor == 0)
        return nullptr;

    rtti_type_descriptor -= 0x10u;

    std::uintptr_t rdata_start = 0u, rdata_size = 0u;
    if (!get_section_info(base_address, ".rdata", rdata_start, rdata_size))
        return nullptr;

    std::vector<std::uintptr_t> xrefs = {};
    {
        std::uintptr_t address = rtti_type_descriptor - base_address;
        std::uintptr_t start = rdata_start;
        const std::uintptr_t end = start + rdata_size;
        while (start != 0 && start < end) {
            std::uintptr_t xref = (std::uintptr_t)g_signatures()->find_signature((void*)start, (void*)(end),
                default_signature::from_bytes(reinterpret_cast<uint8_t*>(&address), sizeof(uint32_t)));
            if (xref == 0)
                break;

            xrefs.push_back(xref);
            start = xref + 4;
        }
    }

    for (const std::uintptr_t xref : xrefs) {
        const std::int32_t offset_from_class = *(int32_t*)(xref - 0x8);
        if (offset_from_class != 0)
            continue;

        std::uintptr_t object_locator = xref - 0xC;

        std::uintptr_t vtable_address = reinterpret_cast<std::uintptr_t>(
            g_signatures()->find_signature((void*)rdata_start, (void*)(rdata_start + rdata_size),
                default_signature::from_bytes(reinterpret_cast<uint8_t*>(&object_locator), sizeof(uint32_t))
            )
        );

        if (vtable_address == 0)
            continue;

        vtable_address += sizeof(std::uintptr_t);

        return (void**)vtable_address;
    }

    return nullptr;
}

void* c_vtables::get_impl(const string_token& hash, bool function, bool do_try) const
{
    (void)function;
    for (auto& vtable : vtables_) {
        if (vtable.hash == hash) {
#ifdef _DEBUG
            assert(vtable.is_vtable == !function);
#endif
            return vtable.address;
        }
    }

    if (do_try)
        return nullptr;

    log::print_error("failed to acquire vtable/signature '{}'", hash);
    std::unreachable();
}

void c_vtables::add(const string_token& name, const xstr& module, const default_vtable& vtable, int index)
{
#if defined(_DEBUG)
    for (auto& vft : vtables_)
        assert(vft.hash != name);
#endif

    void* res = find_vtable(module, vtable);
    if (res == nullptr) {
        log::print_error("failed to find vtable '{}'", name);
#ifdef _DEBUG
        failed_ = true;
        return;
#else
        throw zyra_error("failed to find vtables");
#endif
    }

    void* address = index == -1 ? res :
        reinterpret_cast<void**>(res) + index;

    log::print_debug("vtable '{}' found at {}", name, address);

#ifdef _DEBUG
    vtables_.emplace_back(index == -1, address, name);
#else
    vtables_.emplace_back(address, name);
#endif
}

zyra_end_
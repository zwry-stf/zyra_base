#include "module.h"
#include <Windows.h>


zyra_begin_

void* module::get_export(const char* module_name, const char* proc_name)
{
    void* mod_base = reinterpret_cast<void*>(GetModuleHandleA(module_name));

    const auto base_address = static_cast<const std::uint8_t*>(mod_base);

    const auto dos = static_cast<const IMAGE_DOS_HEADER*>(mod_base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE)
        return nullptr;

    const auto nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(base_address + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE)
        return nullptr;

    const auto* oh = &nt->OptionalHeader;
    const auto export_dir_size = oh->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
    const auto export_dir_address = oh->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

    if (export_dir_size == 0u || export_dir_address == 0u) {
        return nullptr;
    }

    const auto image_export_dir = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(base_address + export_dir_address);
    const auto names_rva = reinterpret_cast<const std::uint32_t*>(base_address + image_export_dir->AddressOfNames);
    const auto name_ordinals_rva = reinterpret_cast<const std::uint16_t*>(base_address + image_export_dir->AddressOfNameOrdinals);
    const auto functions_rva = reinterpret_cast<const std::uint32_t*>(base_address + image_export_dir->AddressOfFunctions);

    DWORD right = image_export_dir->NumberOfNames, left = 0;
    while (right != left) {
        const DWORD middle = left + ((right - left) >> 1);
        if (reinterpret_cast<const char*>(base_address + names_rva[middle]) == nullptr)
            continue;

        const int res = std::strcmp(proc_name, reinterpret_cast<const char*>(base_address + names_rva[middle]));

        if (res == 0) {
            const auto function_rva = functions_rva[name_ordinals_rva[middle]];

            if (function_rva >= export_dir_address &&
                function_rva - export_dir_address < export_dir_size)
                break;

            return const_cast<std::uint8_t*>(base_address) + function_rva;
        }

        if (res > 0)
            left = middle + 1;
        else
            right = middle;
    }

    return nullptr;
}

zyra_end_
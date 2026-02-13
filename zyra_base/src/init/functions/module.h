#pragma once
#include <zyra/util/mem.h>
#include <zyra/util/xstr.h>


zyra_begin_

namespace module {
    [[nodiscard]] void* get_export(const char* module_name, const char* proc_name);

    [[nodiscard]] inline void* get_export(const xstr& module_name, const basic_xstr<char, 156>& proc_name) {
        char buffer_module[xstr::kMaxLength];
        module_name.get(buffer_module);

        char buffer_proc[basic_xstr<char, 156>::kMaxLength];
        proc_name.get(buffer_proc);

        return get_export(buffer_module, buffer_proc);
    }
}

zyra_end_
#include <zyra/init/functions/functions.h>
#include <zyra/util/zyra_error.h>
#include <zyra/util/log.h>
#include "module.h"


zyra_begin_

void* c_functions::get_impl(const string_token& hash) const
{
    for (auto& fn : functions_)
        if (fn.hash == hash)
            return fn.address;

    log::print_error("failed to acquire function '{}'", hash);
    std::unreachable();
}

void c_functions::add_exported(const string_token& name, const xstr& module, const basic_xstr<char, 156>& export_name)
{
#if defined(_DEBUG)
    for (auto& fn : functions_)
        assert(fn.hash != name);
#endif

    void* res = ::zyra::module::get_export(module, export_name);
    if (res == nullptr) {
        log::print_error("failed to find function '{}'", name);
        throw zyra_error("failed to find functions");
    }

    log::print_debug("found function '{}' at {:p}", name, res);

    functions_.emplace_back(res, name);
}

void c_functions::add(const string_token& name, void* address)
{
    assert(address != nullptr);

    functions_.emplace_back(address, name);
}

zyra_end_
#pragma once
#include <zyra/util/class.h>
#include <zyra/util/access.h>
#include <zyra/util/xstr.h>
#include <zyra/util/string_token.h>
#include <vector>
#include <functional>


zyra_begin_

class c_functions {
    zyra_remove_copy_constructor(c_functions);
    zyra_remove_move_constructor(c_functions);

private:
    struct loaded_function_t {
        void* address;
        string_token hash;
    };
    std::vector<loaded_function_t> functions_;

    friend class c_hooks;

public:
    c_functions() = default;

public:
    /*
        usage:
        g_signatures()->initialize_layer(
            [](auto&& add_exported) {
                add_exported("Name", "module.dll", "ExportedFunction");
            }
        );
    */
    template <class Fn>
        requires (std::invocable<Fn,
            std::function<void(const string_token&, const xstr&, const basic_xstr<char, 156>&)>,
            std::function<void(const string_token& name, void* address)>
        >)
    void initialize_layer(Fn&& fn) {
        fn([this](const string_token& name, const xstr& module, const basic_xstr<char, 156>& export_name) {
            this->add_exported(name, module, export_name);
            },
            [this](const string_token& name, void* address) {
                this->add(name, address);
            }
        );
    }

    template <typename T>
        requires (std::is_pointer_v<T>)
    [[nodiscard]] T get(const string_token& hash) const {
        return reinterpret_cast<T>(get_impl(hash));
    }

private:
    [[nodiscard]] void* get_impl(const string_token& hash) const;

private:
    void add(const string_token& name, void* address);
    void add_exported(const string_token& name, const xstr& module, const basic_xstr<char, 156>& export_name);
};

zyra_define_access(c_functions, g_functions);


#define _get_function(_var, _name, _ret, _args) \
static auto _var = ::zyra::g_functions()->get< \
    _ret(*)_args > \
    (_name)

zyra_end_
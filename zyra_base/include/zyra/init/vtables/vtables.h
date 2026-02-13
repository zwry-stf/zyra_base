#pragma once
#include <zyra/util/class.h>
#include <zyra/util/access.h>
#include <zyra/util/string_token.h>
#include "vtable_parser.h"
#include <vector>
#include <zyra/util/xstr.h>
#include <functional>


zyra_begin_

class c_vtables {
    zyra_remove_copy_constructor(c_vtables);
    zyra_remove_move_constructor(c_vtables);

private:
    struct loaded_vtable_t {
#if defined(_DEBUG)
        bool is_vtable; // whether the object is a vtable or a functions location in a vtable
#endif // _DEBUG
        void* address;
        string_token hash;
    };
    std::vector<loaded_vtable_t> vtables_;

    friend class c_hooks;

#if defined(_DEBUG)
    bool failed_;
#endif // _DEBUG

public:
    c_vtables();
    ~c_vtables() = default;

public:
    /*
        usage:
        g_vtables()->initialize_layer(
            [](auto&& add) {
                add("Name", "module.dll", "CClass", 2);
            }
        );
    */
    template <class Fn>
    void initialize_layer(Fn&& fn) {
        fn([this](const string_token& name, const xstr& module, const default_vtable& vtable, int index = -1) {
            this->add(name, module, vtable, index);
            }
        );
        on_init();
    }

    template <typename T>
        requires (std::is_pointer_v<T>)
    [[nodiscard]] zyra_always_inline T get(const string_token& hash) const {
        return reinterpret_cast<T>(get_impl(hash, false));
    }

    template <typename T>
        requires (std::is_pointer_v<T>)
    [[nodiscard]] zyra_always_inline T get_function(const string_token& hash) const {
        return *reinterpret_cast<T*>(get_impl(hash, true));
    }

    template <typename T>
        requires (std::is_pointer_v<T>)
    [[nodiscard]] zyra_always_inline T try_get_function(const string_token& hash) const {
        auto* ret = get_impl(hash, true, true);
        return ret == nullptr ? nullptr : *reinterpret_cast<T*>(ret);
    }

    template <typename T>
        requires (std::is_pointer_v<T>)
    [[nodiscard]] zyra_always_inline T get_function_source(const string_token& hash) const {
        return reinterpret_cast<T>(get_impl(hash, true));
    }

public:
    [[nodiscard]] void* find_vtable(const xstr& module, const default_vtable& vtable);

private:
    [[nodiscard]] void* get_impl(const string_token& hash, bool function, bool do_try = false) const;
    void on_init();

private:
    void add(const string_token& name, const xstr& module, const default_vtable& vtable, int index = -1);
};

zyra_define_access(c_vtables, g_vtables);

zyra_end_
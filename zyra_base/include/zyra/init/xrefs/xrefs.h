#pragma once
#include <zyra/util/class.h>
#include <zyra/util/access.h>
#include <zyra/util/xstr.h>
#include <zyra/util/string_token.h>
#include <vector>
#include <functional>
#include <array>


zyra_begin_

class c_xrefs {
    zyra_remove_copy_constructor(c_xrefs);
    zyra_remove_move_constructor(c_xrefs);

private:
    struct loaded_xref_t {
        void* address;
        string_token hash;
    };
    std::vector<loaded_xref_t> xrefs_;

    friend class c_hooks;

#if defined(_DEBUG)
    bool failed_;
#endif // _DEBUG

public:
    c_xrefs();
    ~c_xrefs() = default;

public:
    /*
        usage:
        g_xrefs()->initialize_layer(
            [](auto&& add) {
                add("Name", "module.dll", "SomeString");
            }
        );
    */
    template <class Fn>
    void initialize_layer(Fn&& fn) {
        fn(
            [this]<std::same_as<basic_xstr<char, 128>>... Args>(const string_token& name, const xstr& module, Args&&... args) {
                this->add(
                    name,
                    module,
                    std::vector<basic_xstr<char, 128>>{ std::forward<Args>(args)... }
                );
            }
        );
        on_init();
    }

    template <typename T>
        requires (std::is_pointer_v<T>)
    [[nodiscard]] zyra_always_inline T get(const string_token& hash) const {
        return reinterpret_cast<T>(get_impl(hash));
    }

    template <typename T>
        requires (std::is_pointer_v<T>)
    [[nodiscard]] zyra_always_inline T try_get(const string_token& hash) const {
        return reinterpret_cast<T>(get_impl(hash, true));
    }

public:
    [[nodiscard]] static void* get_function_by_xref(const xstr& module, const std::vector<basic_xstr<char, 128>>& str);

private:
    [[nodiscard]] void* get_impl(const string_token& hash, bool do_try = false) const;
    void on_init();

private:
    void add(const string_token& name, const xstr& module, const std::vector<basic_xstr<char, 128>>& str);
};

zyra_define_access(c_xrefs, g_xrefs);

zyra_end_
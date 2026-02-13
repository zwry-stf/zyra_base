#pragma once
#include <zyra/util/class.h>
#include <zyra/util/access.h>
#include <zyra/util/string_token.h>
#include <type_traits>
#include <vector>


zyra_begin_

template <typename T>
concept valid_hook =
    std::is_function_v<std::remove_pointer_t<T>>;

class c_hooks {
    zyra_remove_copy_constructor(c_hooks);
    zyra_remove_move_constructor(c_hooks);

private:
    struct inline_hook_t {
        string_token layer_name; // the name of the layer
        void* detour = 0;     // the detour to jump to
        void* address = 0;     // the original functions address
        void* original = 0;     // the original functions trampoline
        string_token name;      // the name of the hook and potentially the name to search for (signatures/vtables/functions) if address is 0
        bool attached{};
    };
    std::vector<inline_hook_t> inline_hooks_;

    struct vtable_hook_t {
        string_token layer_name; // the name of the layer
        void* detour = 0;     // the detour to jump to
        void* source = 0;     // the address where the function pointer will be written to
        void* original = 0;     // the original function
        string_token name;      // the name of the hook
        bool attached{};
    };
    std::vector<vtable_hook_t> vtable_hooks_;

public:
    c_hooks() = default;

public:
    void initalize_layer(const string_token& name);
    void destroy();

public:
    template <valid_hook Fn, typename A>
    void register_inline_hook(Fn& detour, A* target,
                              const string_token& name, const string_token& layer_name) {
        register_inline_hook_impl(
            reinterpret_cast<void**>(&detour),
            reinterpret_cast<void*>(target),
            name,
            layer_name
        );
    }

    template <valid_hook Fn>
    void register_vtable_hook(Fn& detour, const string_token& name,
                              const string_token& layer_name) {
        register_vtable_hook_impl(
            reinterpret_cast<void**>(&detour),
            nullptr, /* source */
            name, 
            layer_name
        );
    }

    template <valid_hook Fn, typename A>
    void register_vtable_hook(Fn& detour, A* source, 
                              const string_token& name, const string_token& layer_name) {
        register_vtable_hook_impl(
            reinterpret_cast<void**>(&detour),
            reinterpret_cast<void*>(source),
            name,
            layer_name
        );
    }

#if not defined(ZYRA_PUBLIC)
    [[nodiscard]] bool should_block_hook(const string_token& name) const;
#endif

public:
    template<valid_hook T>
    [[nodiscard]] T get_original(const string_token& hash) {
        return static_cast<T>(get_original_impl(hash));
    }

private:
    [[nodiscard]] void* get_original_impl(const string_token& hash);

    void register_inline_hook_impl(void** detour, void* target, const string_token& name, const string_token& layer_name);
    void register_vtable_hook_impl(void** detour, void* source, const string_token& name, const string_token& layer_name);
    void pre_init();
    void post_init();

    void remove_hooks();

    bool attach_hook(inline_hook_t& hook);
    bool attach_hook(vtable_hook_t& hook);
};

zyra_define_access(c_hooks, g_hooks);


struct global_inline_hook_loader_t {
    template <valid_hook Fn>
    global_inline_hook_loader_t(Fn& detour, const string_token& name, 
                                string_token layer_name, string_token debug_feature = string_token()) {
#if defined(ZYRA_PUBLIC)
        (void)debug_feature;
#else
        if (g_hooks()->should_block_hook(debug_feature))
            return;
#endif
        g_hooks()->register_inline_hook(detour, (char*)nullptr, name, layer_name);
    }
};

struct global_vtable_hook_loader_t {
    template <valid_hook Fn>
    global_vtable_hook_loader_t(Fn& detour, const string_token& name, 
                                string_token layer_name, string_token debug_feature = string_token()) {
#if defined(ZYRA_PUBLIC)
        (void)debug_feature;
#else
        if (g_hooks()->should_block_hook(debug_feature))
            return;
#endif
        g_hooks()->register_vtable_hook(detour, name, layer_name);
    }
};

#define zyra_define_hook(_name, _ret, _params) \
inline static _ret hk##_name _params

#define zyra_get_original(_name) \
static auto original = ::zyra::g_hooks()->get_original< \
    decltype(&hk##_name)> \
    (#_name)

#define zyra_get_original_ex(_var, _name, _ret, _args) \
static auto _var = ::zyra::g_hooks()->get_original< \
    _ret(*)_args> \
    (_name)

#define zyra_link_inline_hook(_name, _layer_name, _debug_feature) \
namespace { \
    inline static auto _reg_##_name = ::zyra::global_inline_hook_loader_t(hk##_name, #_name, _layer_name, _debug_feature); \
}

#define zyra_link_vtable_hook(_name, _layer_name, _debug_feature) \
namespace { \
    inline static auto _reg_##_name = ::zyra::global_vtable_hook_loader_t(hk##_name, #_name, _layer_name, _debug_feature); \
}

zyra_end_
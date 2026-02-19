#include <zyra/init/hooks/hooks.h>
#include <zyra/util/zyra_error.h>
#include <zyra/util/log.h>
#include <zyra/init/signatures/signatures.h>
#include <zyra/init/vtables/vtables.h>
#include <zyra/init/functions/functions.h>

#include <Windows.h>
#include <Detours/detours.h>


zyra_begin_

void c_hooks::pre_init()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
}

void c_hooks::post_init()
{
    DetourTransactionCommit();
}

void c_hooks::destroy()
{
    pre_init();

    remove_hooks();

    post_init();
}

#if not defined(ZYRA_PUBLIC)
bool c_hooks::should_block_hook(const string_token& name) const
{
    (void)name;
    return false;
}
#endif

void* c_hooks::get_original_impl(const string_token& hash)
{
    for (auto& hook : inline_hooks_)
        if (hook.name == hash)
            return hook.original;

    for (auto& hook : vtable_hooks_)
        if (hook.name == hash)
            return hook.original;

    assert(false && "failed to find hook"); 
    std::unreachable();
}

void c_hooks::register_inline_hook_impl(void** detour, void* target, const string_token& hook_name, 
                                        const string_token& target_name, const string_token& layer_name)
{
#if defined(_DEBUG)
    for (auto& hook : inline_hooks_)
        assert(hook.name != hook_name);
#endif

    inline_hooks_.emplace_back(layer_name, detour, target, nullptr, hook_name, target_name);
}

void c_hooks::register_vtable_hook_impl(void** detour, void* source, const string_token& hook_name,
                                        const string_token& target_name, const string_token& layer_name)
{
#if defined(_DEBUG)
    for (auto& hook : vtable_hooks_)
        assert(hook.name != hook_name);
#endif

    vtable_hooks_.emplace_back(layer_name, detour, source, nullptr, hook_name, target_name);
}

void c_hooks::initalize_layer(const string_token& name)
{
    pre_init();

    for (auto& hook : inline_hooks_) {
        if (hook.layer_name != name)
            continue;

        if (hook.address == nullptr) {
            hook.address = g_signatures()->try_get<void*>(hook.target_name);
            if (hook.address == nullptr) {
                hook.address = g_functions()->get<void*>(hook.target_name); // will throw
            }
        }

        if (!attach_hook(hook))
            throw zyra_error("failed to create hooks");
    }

    for (auto& hook : vtable_hooks_) {
        if (hook.layer_name != name)
            continue;

        if (!attach_hook(hook))
            throw zyra_error("failed to create hooks");
    }

    post_init();
}

void c_hooks::remove_hooks()
{
    for (auto& hook : inline_hooks_) {
        if (!hook.attached)
            continue;
        DetourDetach(&hook.original, hook.detour);
        hook.attached = false;
    }

    for (auto& hook : vtable_hooks_) {
        if (!hook.attached)
            continue;

        assert(hook.source != nullptr);

        DWORD old_protect = 0;
        if (!VirtualProtect(hook.source, sizeof(void*), PAGE_READWRITE, &old_protect)) {
            log::print_warning("failed to remove hook '{}', mem protect failed.", hook.name);
        }
        else {
            InterlockedExchangePointer(
                reinterpret_cast<volatile PVOID*>(hook.source),
                hook.original
            );
            VirtualProtect(hook.source, sizeof(void*), old_protect, &old_protect);
        }
        hook.attached = false;
    }
}

bool c_hooks::attach_hook(inline_hook_t& hook)
{
    hook.original = hook.address;
    auto res = DetourAttach(&hook.original, hook.detour);
    if (res != NO_ERROR) {
        log::print_error("failed to create hook '{}' at {}, error: {}", hook.name, hook.address, res);
        return false;
    }

    hook.attached = true;
    log::print_debug("hook attached '{}' at {}", hook.name, hook.address);

    return true;
}

bool c_hooks::attach_hook(vtable_hook_t& hook)
{
    if (hook.source == nullptr)
        hook.source = g_vtables()->get_function_source<void*>(hook.target_name);

    DWORD old_protect = 0;
    if (!VirtualProtect(hook.source, sizeof(void*), PAGE_READWRITE, &old_protect)) {
        log::print_error("failed to change mem protection for hook '{}' at {}", hook.name, hook.source);
        return false;
    }

    hook.original = (void*)InterlockedExchangePointer(
        reinterpret_cast<volatile PVOID*>(hook.source),
        hook.detour
    );

    if (!VirtualProtect(hook.source, sizeof(void*), old_protect, &old_protect)) {
        log::print_warning("failed to change back protection for hook '{}' at {}", hook.name, hook.source);
    }

    hook.attached = true;
    log::print_debug("hook attached '{}' at {}", hook.name, hook.source);

    return true;
}

zyra_end_
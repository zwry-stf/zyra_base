#pragma once
#include <zyra/util/def.h>
#include <cstdint>
#include <cstddef>
#include <memory>


#if not defined(zyra_mem_pad)
#define _paste_impl(a, b) a##b
#define _paste(a, b) _paste_impl(a, b)
#define zyra_mem_pad(_size) char _paste(pad, __COUNTER__)[_size]
#endif // zyra_mem_pad

zyra_begin_

namespace mem {
    template <typename T, std::uint32_t Index, class B, typename... Args>
    [[nodiscard]] zyra_always_inline T call_v_func(B* thisptr, Args... args) {
        using fn = T(__fastcall*)(B*, Args...);
        auto arr = *reinterpret_cast<fn* const*>(thisptr);
        return arr[Index](thisptr, std::forward<Args>(args)...);
    }

    template <typename T = void*>
    [[nodiscard]] zyra_always_inline T& get_v_func(const void* thisptr, std::size_t index) {
        return (*static_cast<T* const*>(thisptr))[index];
    }

    template <typename T>
    [[nodiscard]] zyra_always_inline void* resolve_relative_address(T* address, std::uint32_t rva_offset, std::uint32_t rip_offset) {
        const auto rva = *reinterpret_cast<uint32_t*>(
            reinterpret_cast<std::uintptr_t>(address) + rva_offset
        );
        const auto rip = reinterpret_cast<std::uintptr_t>(address) + rip_offset;

        return reinterpret_cast<void*>(rva + rip);
    }
}

zyra_end_
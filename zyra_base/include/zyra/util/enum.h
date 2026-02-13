#pragma once
#include <utility>
#include <type_traits>


// define & | and ~ operators
#define zyra_define_enum_operators(_enum) \
static_assert(std::is_enum_v<_enum>);                                              \
inline constexpr _enum operator&(const _enum& a, const _enum& b) noexcept {        \
    return static_cast<_enum>(                                                     \
        std::to_underlying(a) & std::to_underlying(b)                              \
    );                                                                             \
}                                                                                  \
inline constexpr _enum& operator&=(_enum& a, const _enum& b) noexcept {            \
    a = a & b;                                                                     \
    return a;                                                                      \
}                                                                                  \
inline constexpr _enum operator|(const _enum& a, const _enum& b) noexcept {        \
    return static_cast<_enum>(                                                     \
        std::to_underlying(a) | std::to_underlying(b)                              \
    );                                                                             \
}                                                                                  \
inline constexpr _enum& operator|=(_enum& a, const _enum& b) noexcept {            \
    a = a | b;                                                                     \
    return a;                                                                      \
}                                                                                  \
inline constexpr _enum operator~(const _enum& a) noexcept {                        \
    return static_cast<_enum>(                                                     \
        ~std::to_underlying(a)                                                     \
    );                                                                             \
}
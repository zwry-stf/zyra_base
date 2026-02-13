#pragma once
#include <zyra/util/def.h>
#include <zyra/util/math_constants.h>
#include <cmath>
#include <algorithm>
#include <type_traits>


zyra_begin_

template <typename _Type>
concept vec4_type = std::is_floating_point_v<_Type>;

template <vec4_type _Type>
class vec4_basic {
private:
    inline static constexpr _Type kNull = static_cast<_Type>(0);

public:
    _Type x, y, z, w;

public:
    zyra_always_inline constexpr vec4_basic();
    zyra_always_inline constexpr vec4_basic(const _Type x, const _Type y, const _Type z, const _Type w);
    zyra_always_inline constexpr explicit vec4_basic(const _Type v);

    constexpr vec4_basic(const vec4_basic&) = default;
    constexpr vec4_basic(vec4_basic&&) = default;
    constexpr vec4_basic& operator=(const vec4_basic&) = default;
    constexpr vec4_basic& operator=(vec4_basic&&) = default;
    ~vec4_basic() = default;

public:
    zyra_always_inline constexpr vec4_basic<_Type>& operator+=(const vec4_basic<_Type>& v);
    zyra_always_inline constexpr vec4_basic<_Type>& operator-=(const vec4_basic<_Type>& v);
    zyra_always_inline constexpr vec4_basic<_Type>& operator*=(const vec4_basic<_Type>& v);
    zyra_always_inline constexpr vec4_basic<_Type>& operator/=(const vec4_basic<_Type>& v);
    [[nodiscard]] zyra_always_inline constexpr vec4_basic<_Type> operator+(const vec4_basic<_Type>& v) const;
    [[nodiscard]] zyra_always_inline constexpr vec4_basic<_Type> operator-(const vec4_basic<_Type>& v) const;
    [[nodiscard]] zyra_always_inline constexpr vec4_basic<_Type> operator*(const vec4_basic<_Type>& v) const;
    [[nodiscard]] zyra_always_inline constexpr vec4_basic<_Type> operator/(const vec4_basic<_Type>& v) const;

    zyra_always_inline constexpr vec4_basic<_Type>& operator+=(const _Type v);
    zyra_always_inline constexpr vec4_basic<_Type>& operator-=(const _Type v);
    zyra_always_inline constexpr vec4_basic<_Type>& operator*=(const _Type v);
    zyra_always_inline constexpr vec4_basic<_Type>& operator/=(const _Type v);
    [[nodiscard]] zyra_always_inline constexpr vec4_basic<_Type> operator+(const _Type v) const;
    [[nodiscard]] zyra_always_inline constexpr vec4_basic<_Type> operator-(const _Type v) const;
    [[nodiscard]] zyra_always_inline constexpr vec4_basic<_Type> operator*(const _Type v) const;
    [[nodiscard]] zyra_always_inline constexpr vec4_basic<_Type> operator/(const _Type v) const;

    [[nodiscard]] zyra_always_inline constexpr vec4_basic<_Type> operator-() const;

    [[nodiscard]] zyra_always_inline constexpr bool operator==(const vec4_basic<_Type>& v) const;
    [[nodiscard]] zyra_always_inline constexpr bool operator!=(const vec4_basic<_Type>& v) const;
    [[nodiscard]] zyra_always_inline constexpr bool operator==(const _Type v) const;
    [[nodiscard]] zyra_always_inline constexpr bool operator!=(const _Type v) const;

public:
    [[nodiscard]] zyra_always_inline _Type length() const;
    [[nodiscard]] zyra_always_inline _Type length_2d() const;
    [[nodiscard]] zyra_always_inline vec4_basic<_Type> normalize() const;

    [[nodiscard]] zyra_always_inline constexpr _Type dot(const vec4_basic<_Type>& v) const;

    [[nodiscard]] zyra_always_inline constexpr vec4_basic<_Type> to_rad() const;
    [[nodiscard]] zyra_always_inline constexpr vec4_basic<_Type> to_deg() const;

    [[nodiscard]] zyra_always_inline constexpr bool is_zero() const;

    [[nodiscard]] zyra_always_inline constexpr vec4_basic<_Type> lerp(const vec4_basic<_Type>& v, const _Type t) const;
};

using vec4 = vec4_basic<float>;

zyra_end_

#include "vec4.inl"
#pragma once
#include <zyra/util/def.h>
#include <zyra/util/math_constants.h>
#include <cmath>
#include <algorithm>
#include <type_traits>


zyra_begin_

template <typename _Type>
concept vec3_type = std::is_floating_point_v<_Type>;

template <vec3_type _Type>
class vec3_basic {
private:
    inline static constexpr _Type kNull = static_cast<_Type>(0);

public:
    _Type x, y, z;

public:
    zyra_always_inline constexpr vec3_basic();
    zyra_always_inline constexpr vec3_basic(const _Type x, const _Type y, const _Type z);
    zyra_always_inline constexpr explicit vec3_basic(const _Type v);

    constexpr vec3_basic(const vec3_basic&) = default;
    constexpr vec3_basic(vec3_basic&&) = default;
    constexpr vec3_basic& operator=(const vec3_basic&) = default;
    constexpr vec3_basic& operator=(vec3_basic&&) = default;
    ~vec3_basic() = default;

public:
    zyra_always_inline constexpr vec3_basic<_Type>& operator+=(const vec3_basic<_Type>& v);
    zyra_always_inline constexpr vec3_basic<_Type>& operator-=(const vec3_basic<_Type>& v);
    zyra_always_inline constexpr vec3_basic<_Type>& operator*=(const vec3_basic<_Type>& v);
    zyra_always_inline constexpr vec3_basic<_Type>& operator/=(const vec3_basic<_Type>& v);
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<_Type>  operator+(const vec3_basic<_Type>& v) const;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<_Type>  operator-(const vec3_basic<_Type>& v) const;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<_Type>  operator*(const vec3_basic<_Type>& v) const;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<_Type>  operator/(const vec3_basic<_Type>& v) const;

    zyra_always_inline constexpr vec3_basic<_Type>& operator+=(const _Type v);
    zyra_always_inline constexpr vec3_basic<_Type>& operator-=(const _Type v);
    zyra_always_inline constexpr vec3_basic<_Type>& operator*=(const _Type v);
    zyra_always_inline constexpr vec3_basic<_Type>& operator/=(const _Type v);
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<_Type>  operator+(const _Type v) const;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<_Type>  operator-(const _Type v) const;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<_Type>  operator*(const _Type v) const;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<_Type>  operator/(const _Type v) const;

    [[nodiscard]] zyra_always_inline constexpr vec3_basic<_Type>  operator-() const;

    [[nodiscard]] zyra_always_inline constexpr bool operator==(const vec3_basic<_Type>& v) const;
    [[nodiscard]] zyra_always_inline constexpr bool operator!=(const vec3_basic<_Type>& v) const;
    [[nodiscard]] zyra_always_inline constexpr bool operator==(const _Type v) const;
    [[nodiscard]] zyra_always_inline constexpr bool operator!=(const _Type v) const;

public:
    [[nodiscard]] zyra_always_inline _Type length() const;
    [[nodiscard]] zyra_always_inline _Type length_2d() const;
    [[nodiscard]] zyra_always_inline vec3_basic<_Type> normalize() const;
    [[nodiscard]] zyra_always_inline vec3_basic<_Type> normalize_angle() const;

    [[nodiscard]] zyra_always_inline constexpr _Type dot(const vec3_basic<_Type>& v) const;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<_Type> cross(const vec3_basic<_Type>& v) const;

    [[nodiscard]] zyra_always_inline constexpr vec3_basic<_Type> to_rad() const;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<_Type> to_deg() const;

    [[nodiscard]] zyra_always_inline constexpr bool is_zero() const;

    [[nodiscard]] zyra_always_inline constexpr vec3_basic<_Type> lerp(const vec3_basic<_Type>& v, const _Type t) const;

    zyra_always_inline void to_directions(vec3_basic<_Type>* forward, vec3_basic<_Type>* right, vec3_basic<_Type>* up) const;

    [[nodiscard]] zyra_always_inline _Type get_angle_x() const;
    [[nodiscard]] zyra_always_inline _Type get_angle_y() const;
    [[nodiscard]] zyra_always_inline vec3_basic<_Type> to_angles() const;
};


using vec3 = vec3_basic<float>;

#include "vec3.inl"

zyra_end_
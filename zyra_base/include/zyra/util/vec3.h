#pragma once
#include <zyra/util/def.h>
#include <zyra/util/math_constants.h>
#include <cmath>
#include <algorithm>
#include <type_traits>


zyra_begin_

template <typename Type>
concept vec3_type = std::is_floating_point_v<Type>;

template <vec3_type Type>
class vec3_basic {
private:
    inline static constexpr Type kNull = static_cast<Type>(0);

public:
    Type x, y, z;

public:
    zyra_always_inline constexpr vec3_basic() noexcept;
    zyra_always_inline constexpr vec3_basic(const Type x, const Type y, const Type z) noexcept;
    zyra_always_inline constexpr explicit vec3_basic(const Type v) noexcept;

    zyra_always_inline constexpr vec3_basic(const vec3_basic&) noexcept = default;
    zyra_always_inline constexpr vec3_basic(vec3_basic&&) noexcept = default;
    zyra_always_inline constexpr vec3_basic& operator=(const vec3_basic&) noexcept = default;
    zyra_always_inline constexpr vec3_basic& operator=(vec3_basic&&) noexcept = default;
    zyra_always_inline ~vec3_basic() noexcept = default;

public:
    zyra_always_inline constexpr vec3_basic<Type>& operator+=(const vec3_basic<Type>& v) noexcept;
    zyra_always_inline constexpr vec3_basic<Type>& operator-=(const vec3_basic<Type>& v) noexcept;
    zyra_always_inline constexpr vec3_basic<Type>& operator*=(const vec3_basic<Type>& v) noexcept;
    zyra_always_inline constexpr vec3_basic<Type>& operator/=(const vec3_basic<Type>& v) noexcept;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<Type>  operator+(const vec3_basic<Type>& v) const noexcept;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<Type>  operator-(const vec3_basic<Type>& v) const noexcept;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<Type>  operator*(const vec3_basic<Type>& v) const noexcept;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<Type>  operator/(const vec3_basic<Type>& v) const noexcept;

    zyra_always_inline constexpr vec3_basic<Type>& operator+=(const Type v) noexcept;
    zyra_always_inline constexpr vec3_basic<Type>& operator-=(const Type v) noexcept;
    zyra_always_inline constexpr vec3_basic<Type>& operator*=(const Type v) noexcept;
    zyra_always_inline constexpr vec3_basic<Type>& operator/=(const Type v) noexcept;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<Type>  operator+(const Type v) const noexcept;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<Type>  operator-(const Type v) const noexcept;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<Type>  operator*(const Type v) const noexcept;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<Type>  operator/(const Type v) const noexcept;

    [[nodiscard]] zyra_always_inline constexpr vec3_basic<Type>  operator-() const noexcept;

    [[nodiscard]] zyra_always_inline constexpr bool operator==(const vec3_basic<Type>& v) const noexcept;
    [[nodiscard]] zyra_always_inline constexpr bool operator!=(const vec3_basic<Type>& v) const noexcept;
    [[nodiscard]] zyra_always_inline constexpr bool operator==(const Type v) const noexcept;
    [[nodiscard]] zyra_always_inline constexpr bool operator!=(const Type v) const noexcept;

public:
    [[nodiscard]] zyra_always_inline Type length() const noexcept;
    [[nodiscard]] zyra_always_inline Type length_2d() const noexcept;
    [[nodiscard]] zyra_always_inline vec3_basic<Type> normalize() const noexcept;
    [[nodiscard]] zyra_always_inline vec3_basic<Type> normalize_angle() const noexcept;

    [[nodiscard]] zyra_always_inline constexpr Type dot(const vec3_basic<Type>& v) const noexcept;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<Type> cross(const vec3_basic<Type>& v) const noexcept;

    [[nodiscard]] zyra_always_inline constexpr vec3_basic<Type> to_rad() const noexcept;
    [[nodiscard]] zyra_always_inline constexpr vec3_basic<Type> to_deg() const noexcept;

    [[nodiscard]] zyra_always_inline constexpr bool is_zero() const noexcept;

    [[nodiscard]] zyra_always_inline constexpr vec3_basic<Type> lerp(const vec3_basic<Type>& v, const Type t) const noexcept;

    zyra_always_inline void to_directions(vec3_basic<Type>* forward, vec3_basic<Type>* right, vec3_basic<Type>* up) const noexcept;
    zyra_always_inline void angle_vectors(vec3_basic<Type>* forward, vec3_basic<Type>* left, vec3_basic<Type>* up) const noexcept;

    [[nodiscard]] zyra_always_inline Type get_angle_x() const noexcept;
    [[nodiscard]] zyra_always_inline Type get_angle_y() const noexcept;
    [[nodiscard]] zyra_always_inline vec3_basic<Type> to_angles() const noexcept;
};

using vec3 = vec3_basic<float>;

template <vec3_type Type>
[[nodiscard]] zyra_always_inline constexpr vec3_basic<Type> operator+(float a, const vec3_basic<Type>& b) {
    return vec3_basic<Type>(a) + b;
}
template <vec3_type Type>
[[nodiscard]] zyra_always_inline constexpr vec3_basic<Type> operator-(float a, const vec3_basic<Type>& b) {
    return vec3_basic<Type>(a) - b;
}
template <vec3_type Type>
[[nodiscard]] zyra_always_inline constexpr vec3_basic<Type> operator*(float a, const vec3_basic<Type>& b) {
    return vec3_basic<Type>(a) * b;
}
template <vec3_type Type>
[[nodiscard]] zyra_always_inline constexpr vec3_basic<Type> operator/(float a, const vec3_basic<Type>& b) {
    return vec3_basic<Type>(a) / b;
}

zyra_end_

#include "vec3.inl"
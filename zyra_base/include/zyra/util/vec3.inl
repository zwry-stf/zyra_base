#pragma once

#ifndef assert
#include <assert.h>
#endif


zyra_begin_

// constructors
template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>::vec3_basic() noexcept
    : x(kNull), y(kNull), z(kNull) 
{
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>::vec3_basic(const _Type x, const _Type y, const _Type z) noexcept
    : x(x), y(y), z(z)
{
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>::vec3_basic(const _Type v) noexcept
    : x(v), y(v), z(v)
{
}

// math operators
template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator+=(const vec3_basic<_Type>& v) noexcept
{
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator-=(const vec3_basic<_Type>& v) noexcept
{
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator*=(const vec3_basic<_Type>& v) noexcept
{
    x *= v.x;
    y *= v.y;
    z *= v.z;

    return *this;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator/=(const vec3_basic<_Type>& v) noexcept
{
    x /= v.x;
    y /= v.y;
    z /= v.z;

    return *this;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator+(const vec3_basic<_Type>& v) const noexcept
{
    return vec3_basic<_Type>(
        x + v.x,
        y + v.y,
        z + v.z
    );
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator-(const vec3_basic<_Type>& v) const noexcept
{
    return vec3_basic<_Type>(
        x - v.x,
        y - v.y,
        z - v.z
    );
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator*(const vec3_basic<_Type>& v) const noexcept
{
    return vec3_basic<_Type>(
        x * v.x,
        y * v.y,
        z * v.z
    );
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator/(const vec3_basic<_Type>& v) const noexcept
{
    return vec3_basic<_Type>(
        x / v.x,
        y / v.y,
        z / v.z
    );
}


template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator+=(const _Type v) noexcept
{
    x += v;
    y += v;
    z += v;

    return *this;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator-=(const _Type v) noexcept
{
    x -= v;
    y -= v;
    z -= v;

    return *this;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator*=(const _Type v) noexcept
{
    x *= v;
    y *= v;
    z *= v;

    return *this;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator/=(const _Type v) noexcept
{
    const _Type i_v = _Type(1) / v;

    return *this *= i_v;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator+(const _Type v) const noexcept
{
    return vec3_basic<_Type>(
        x + v,
        y + v,
        z + v
    );
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator-(const _Type v) const noexcept
{
    return vec3_basic<_Type>(
        x - v,
        y - v,
        z - v
    );
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator*(const _Type v) const noexcept
{
    return vec3_basic<_Type>(
        x * v,
        y * v,
        z * v
    );
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator/(const _Type v) const noexcept
{
    const _Type i_v = _Type(1) / v;

    return *this * i_v;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator-() const noexcept
{
    return vec3_basic<_Type>(
        -x,
        -y,
        -z
    );
}

// comparison operators
template<vec3_type _Type>
zyra_always_inline constexpr bool vec3_basic<_Type>::operator==(const vec3_basic<_Type>& v) const noexcept
{
    return x == v.x &&
        y == v.y &&
        z == v.z;
}

template<vec3_type _Type>
zyra_always_inline constexpr bool vec3_basic<_Type>::operator!=(const vec3_basic<_Type>& v) const noexcept
{
    return !(*this == v);
}

template<vec3_type _Type>
zyra_always_inline constexpr bool vec3_basic<_Type>::operator==(const _Type v) const noexcept
{
    return x == v &&
        y == v &&
        z == v;
}

template<vec3_type _Type>
zyra_always_inline constexpr bool vec3_basic<_Type>::operator!=(const _Type v) const noexcept
{
    return !(*this == v);
}

// math functions
template<vec3_type _Type>
inline _Type vec3_basic<_Type>::length() const noexcept
{
    return std::sqrt(x * x + y * y + z * z);
}

template<vec3_type _Type>
zyra_always_inline _Type vec3_basic<_Type>::length_2d() const noexcept
{
    return std::sqrt(x * x + y * y);
}

template<vec3_type _Type>
zyra_always_inline vec3_basic<_Type> vec3_basic<_Type>::normalize() const noexcept
{
    const auto len = length();
    if (len < _Type(1e-10)) {
        return vec3_basic<_Type>();
    }

    const _Type i_len = _Type(1) / len;

    return *this * i_len;
}

template<vec3_type _Type>
inline zyra_always_inline vec3_basic<_Type> vec3_basic<_Type>::normalize_angle() const noexcept
{
    vec3_basic<_Type> ret = *this;
    assert(std::isfinite(ret.x));
    assert(std::isfinite(ret.y));
    while (ret.y > _Type(180)) ret.y -= _Type(360);
    while (ret.y < _Type(-180)) ret.y += _Type(360);
    ret.x = std::clamp<_Type>(ret.x, _Type(-89), _Type(89));

    return ret;
}

template<vec3_type _Type>
zyra_always_inline constexpr _Type vec3_basic<_Type>::dot(const vec3_basic<_Type>& v) const noexcept
{
    return x * v.x + y * v.y + z * v.z;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::cross(const vec3_basic<_Type>& v) const noexcept
{
    return vec3_basic<_Type>(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::to_rad() const noexcept
{
    return *this * static_cast<_Type>(math::constants::k_d2r);
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::to_deg() const noexcept
{
    return *this * static_cast<_Type>(math::constants::k_r2d);
}

template<vec3_type _Type>
zyra_always_inline constexpr bool vec3_basic<_Type>::is_zero() const noexcept
{
    return *this == kNull;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::lerp(const vec3_basic<_Type>& v, const _Type t) const noexcept
{
    return *this + (v - *this) * t;
}

template<vec3_type _Type>
zyra_always_inline void vec3_basic<_Type>::to_directions(vec3_basic<_Type>* forward, vec3_basic<_Type>* right, vec3_basic<_Type>* up) const noexcept
{
    const _Type angle_rad_x = x * static_cast<_Type>(math::constants::k_d2r);
    const _Type angle_rad_y = y * static_cast<_Type>(math::constants::k_d2r);

    const _Type sx = std::sin(angle_rad_x), cx = std::cos(angle_rad_x);
    const _Type sy = std::sin(angle_rad_y), cy = std::cos(angle_rad_y);

    if (forward != nullptr) {
        *forward = vec3_basic<_Type>{ cx * cy, cx * sy, -sx };
    }

    if (right != nullptr) {
        *right = vec3_basic<_Type>{ sy * cx, -cy * cx, sx };
    }

    if (up != nullptr) {
        *up = vec3_basic<_Type>{ sx * cy, sx * sy, cx };
    }
}

template<vec3_type Type>
inline zyra_always_inline void vec3_basic<Type>::angle_vectors(vec3_basic<Type>* forward, vec3_basic<Type>* left, vec3_basic<Type>* up) const noexcept
{
    const Type pitch = this->x * static_cast<Type>(math::constants::k_d2r);
    const Type yaw = this->y * static_cast<Type>(math::constants::k_d2r);
    const Type roll = this->z * static_cast<Type>(math::constants::k_d2r);

    const Type sx = std::sin(pitch);
    const Type cx = std::cos(pitch);
    const Type sy = std::sin(yaw);
    const Type cy = std::cos(yaw);
    const Type sz = std::sin(roll);
    const Type cz = std::cos(roll);

    if (forward) {
        forward->x = cy * cx;
        forward->y = sy * cx;
        forward->z = -sx;
    }

    if (left) {
        left->x = (sz * sx * cy) - (cz * sy);
        left->y = (sz * sx * sy) + (cz * cy);
        left->z = sz * cx;
    }

    if (up) {
        up->x = (cz * sx * cy) + (sy * sz);
        up->y = (cz * sx * sy) - (cz * cy);
        up->z = cz * cx;
    }
}

template<vec3_type _Type>
zyra_always_inline _Type vec3_basic<_Type>::get_angle_x() const noexcept
{
    const _Type l2d = length_2d();
    if (l2d < _Type(1e-12))
        return kNull;

    const _Type pitch = std::atan2(-z, l2d) * static_cast<_Type>(math::constants::k_r2d);

    return std::clamp(
        pitch,
        _Type(-89),
        _Type(89)
    );
}

template<vec3_type _Type>
zyra_always_inline _Type vec3_basic<_Type>::get_angle_y() const noexcept
{
    _Type yaw = std::atan2f(y, x) * static_cast<_Type>(math::constants::k_r2d);

    while (yaw > _Type(180)) yaw -= _Type(360);
    while (yaw < _Type(-180)) yaw += _Type(360);

    return yaw;
}

template<vec3_type _Type>
zyra_always_inline vec3_basic<_Type> vec3_basic<_Type>::to_angles() const noexcept
{
    return vec3_basic<_Type>(
        this->get_angle_x(),
        this->get_angle_y(),
        kNull
    );
}

zyra_end_
#pragma once


zyra_begin_

// constructors
template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>::vec3_basic()
    : x(kNull), y(kNull), z(kNull) 
{
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>::vec3_basic(const _Type x, const _Type y, const _Type z)
    : x(x), y(y), z(z)
{
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>::vec3_basic(const _Type v)
    : x(v), y(v), z(v)
{
}

// math operators
template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator+=(const vec3_basic<_Type>& v)
{
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator-=(const vec3_basic<_Type>& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator*=(const vec3_basic<_Type>& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;

    return *this;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator/=(const vec3_basic<_Type>& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;

    return *this;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator+(const vec3_basic<_Type>& v) const
{
    return vec3_basic<_Type>(
        x + v.x,
        y + v.y,
        z + v.z
    );
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator-(const vec3_basic<_Type>& v) const
{
    return vec3_basic<_Type>(
        x - v.x,
        y - v.y,
        z - v.z
    );
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator*(const vec3_basic<_Type>& v) const
{
    return vec3_basic<_Type>(
        x * v.x,
        y * v.y,
        z * v.z
    );
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator/(const vec3_basic<_Type>& v) const
{
    return vec3_basic<_Type>(
        x / v.x,
        y / v.y,
        z / v.z
    );
}


template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator+=(const _Type v)
{
    x += v;
    y += v;
    z += v;

    return *this;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator-=(const _Type v)
{
    x -= v;
    y -= v;
    z -= v;

    return *this;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator*=(const _Type v)
{
    x *= v;
    y *= v;
    z *= v;

    return *this;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type>& vec3_basic<_Type>::operator/=(const _Type v)
{
    const _Type i_v = _Type(1) / v;

    return *this *= i_v;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator+(const _Type v) const
{
    return vec3_basic<_Type>(
        x + v,
        y + v,
        z + v
    );
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator-(const _Type v) const
{
    return vec3_basic<_Type>(
        x - v,
        y - v,
        z - v
    );
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator*(const _Type v) const
{
    return vec3_basic<_Type>(
        x * v,
        y * v,
        z * v
    );
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator/(const _Type v) const
{
    const _Type i_v = _Type(1) / v;

    return *this * i_v;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::operator-() const
{
    return vec3_basic<_Type>(
        -x,
        -y,
        -z
    );
}

// comparison operators
template<vec3_type _Type>
zyra_always_inline constexpr bool vec3_basic<_Type>::operator==(const vec3_basic<_Type>& v) const
{
    return x == v.x &&
        y == v.y &&
        z == v.z;
}

template<vec3_type _Type>
zyra_always_inline constexpr bool vec3_basic<_Type>::operator!=(const vec3_basic<_Type>& v) const
{
    return !(*this == v);
}

template<vec3_type _Type>
zyra_always_inline constexpr bool vec3_basic<_Type>::operator==(const _Type v) const
{
    return x == v &&
        y == v &&
        z == v;
}

template<vec3_type _Type>
zyra_always_inline constexpr bool vec3_basic<_Type>::operator!=(const _Type v) const
{
    return !(*this == v);
}

// math functions
template<vec3_type _Type>
inline _Type vec3_basic<_Type>::length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

template<vec3_type _Type>
zyra_always_inline _Type vec3_basic<_Type>::length_2d() const
{
    return std::sqrt(x * x + y * y);
}

template<vec3_type _Type>
zyra_always_inline vec3_basic<_Type> vec3_basic<_Type>::normalize() const
{
    const _Type i_len = _Type(1) / length();

    return *this * i_len;
}

template<vec3_type _Type>
inline zyra_always_inline vec3_basic<_Type> vec3_basic<_Type>::normalize_angle() const
{
    vec3_basic<_Type> ret = *this;
    while (ret.y > _Type(180)) ret.y -= _Type(360);
    while (ret.y < _Type(-180)) ret.y += _Type(360);
    ret.x = std::clamp<_Type>(ret.x, _Type(-89), _Type(89));

    return ret;
}

template<vec3_type _Type>
zyra_always_inline constexpr _Type vec3_basic<_Type>::dot(const vec3_basic<_Type>& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::cross(const vec3_basic<_Type>& v) const
{
    return vec3_basic<_Type>(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::to_rad() const
{
    return *this * static_cast<_Type>(math::constants::k_d2r);
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::to_deg() const
{
    return *this * static_cast<_Type>(math::constants::k_r2d);
}

template<vec3_type _Type>
zyra_always_inline constexpr bool vec3_basic<_Type>::is_zero() const
{
    return *this == kNull;
}

template<vec3_type _Type>
zyra_always_inline constexpr vec3_basic<_Type> vec3_basic<_Type>::lerp(const vec3_basic<_Type>& v, const _Type t) const
{
    return *this + (v - *this) * t;
}

template<vec3_type _Type>
zyra_always_inline void vec3_basic<_Type>::to_directions(vec3_basic<_Type>* forward, vec3_basic<_Type>* right, vec3_basic<_Type>* up) const
{
    const _Type angle_rad_x = x * static_cast<_Type>(math::constants::k_d2r);
    const _Type angle_rad_y = y * static_cast<_Type>(math::constants::k_d2r);

    const _Type sp = std::sin(angle_rad_x), cp = std::cos(angle_rad_x);
    const _Type sy = std::sin(angle_rad_y), cy = std::cos(angle_rad_y);

    if (forward != nullptr)
        *forward = vec3_basic<_Type>{ cp * cy, cp * sy, -sp };

    if (right != nullptr)
        *right = vec3_basic<_Type>{ sy * cp, -cy * cp, sp };

    if (up != nullptr)
        *up = vec3_basic<_Type>{ sp * cy, sp * sy, cp };
}

template<vec3_type _Type>
zyra_always_inline _Type vec3_basic<_Type>::get_angle_x() const
{
    const _Type l2d = length_2d();
    if (l2d < _Type(1e-7))
        return kNull;

    const _Type pitch = std::atan2(-z, l2d) * static_cast<_Type>(math::constants::k_r2d);

    return std::clamp(
        pitch,
        _Type(-89),
        _Type(89)
    );
}

template<vec3_type _Type>
zyra_always_inline _Type vec3_basic<_Type>::get_angle_y() const
{
    const _Type l2d = length_2d();
    if (l2d < static_cast<_Type>(1e-7))
        return kNull;

    _Type yaw = std::atan2f(y, x) * static_cast<_Type>(math::constants::k_r2d);

    while (yaw > _Type(180)) yaw -= _Type(360);
    while (yaw < _Type(-180)) yaw += _Type(360);

    return yaw;
}

template<vec3_type _Type>
zyra_always_inline vec3_basic<_Type> vec3_basic<_Type>::to_angles() const
{
    return vec3_basic<_Type>(
        this->get_angle_x(),
        this->get_angle_y(),
        kNull
    );
}

zyra_end_
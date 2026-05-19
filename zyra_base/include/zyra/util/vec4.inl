#pragma once


zyra_begin_

// constructors
template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type>::vec4_basic()
    : x(kNull), y(kNull), z(kNull), w(kNull)
{
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type>::vec4_basic(const _Type x, const _Type y, const _Type z, const _Type w)
    : x(x), y(y), z(z), w(w)
{
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type>::vec4_basic(const _Type v)
    : x(v), y(v), z(v), w(v)
{
}

// math operators
template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type>& vec4_basic<_Type>::operator+=(const vec4_basic<_Type>& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;

    return *this;
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type>& vec4_basic<_Type>::operator-=(const vec4_basic<_Type>& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;

    return *this;
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type>& vec4_basic<_Type>::operator*=(const vec4_basic<_Type>& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;

    return *this;
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type>& vec4_basic<_Type>::operator/=(const vec4_basic<_Type>& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;

    return *this;
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type> vec4_basic<_Type>::operator+(const vec4_basic<_Type>& v) const
{
    return vec4_basic<_Type>(
        x + v.x,
        y + v.y,
        z + v.z,
        w + v.w
    );
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type> vec4_basic<_Type>::operator-(const vec4_basic<_Type>& v) const
{
    return vec4_basic<_Type>(
        x - v.x,
        y - v.y,
        z - v.z, 
        w - v.w
    );
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type> vec4_basic<_Type>::operator*(const vec4_basic<_Type>& v) const
{
    return vec4_basic<_Type>(
        x * v.x,
        y * v.y,
        z * v.z,
        w * v.w
    );
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type> vec4_basic<_Type>::operator/(const vec4_basic<_Type>& v) const
{
    return vec4_basic<_Type>(
        x / v.x,
        y / v.y,
        z / v.z,
        w / v.w
    );
}


template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type>& vec4_basic<_Type>::operator+=(const _Type v)
{
    x += v;
    y += v;
    z += v;
    w += v;

    return *this;
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type>& vec4_basic<_Type>::operator-=(const _Type v)
{
    x -= v;
    y -= v;
    z -= v;
    w -= v;

    return *this;
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type>& vec4_basic<_Type>::operator*=(const _Type v)
{
    x *= v;
    y *= v;
    z *= v;
    w *= v;

    return *this;
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type>& vec4_basic<_Type>::operator/=(const _Type v)
{
    const _Type i_v = static_cast<_Type>(1) / v;

    return *this *= i_v;
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type> vec4_basic<_Type>::operator+(const _Type v) const
{
    return vec4_basic<_Type>(
        x + v,
        y + v,
        z + v,
        w + v
    );
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type> vec4_basic<_Type>::operator-(const _Type v) const
{
    return vec4_basic<_Type>(
        x - v,
        y - v,
        z - v,
        w - v
    );
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type> vec4_basic<_Type>::operator*(const _Type v) const
{
    return vec4_basic<_Type>(
        x * v,
        y * v,
        z * v,
        w * v
    );
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type> vec4_basic<_Type>::operator/(const _Type v) const
{
    const _Type i_v = static_cast<_Type>(1) / v;

    return *this * i_v;
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type> vec4_basic<_Type>::operator-() const
{
    return vec4_basic<_Type>(
        -x,
        -y,
        -z,
        -w
    );
}

// comparison operators
template<vec4_type _Type>
zyra_always_inline constexpr bool vec4_basic<_Type>::operator==(const vec4_basic<_Type>& v) const
{
    return x == v.x &&
        y == v.y &&
        z == v.z &&
        w == v.w;
}

template<vec4_type _Type>
zyra_always_inline constexpr bool vec4_basic<_Type>::operator!=(const vec4_basic<_Type>& v) const
{
    return !(*this == v);
}

template<vec4_type _Type>
zyra_always_inline constexpr bool vec4_basic<_Type>::operator==(const _Type v) const
{
    return x == v &&
        y == v &&
        z == v &&
        w == v;
}

template<vec4_type _Type>
zyra_always_inline constexpr bool vec4_basic<_Type>::operator!=(const _Type v) const
{
    return !(*this == v);
}

// math functions
template<vec4_type _Type>
zyra_always_inline _Type vec4_basic<_Type>::length() const
{
    return std::sqrt(x * x + y * y + z * z + w * w);
}

template<vec4_type _Type>
zyra_always_inline _Type vec4_basic<_Type>::length_2d() const
{
    return std::sqrt(x * x + y * y);
}

template<vec4_type _Type>
zyra_always_inline vec4_basic<_Type> vec4_basic<_Type>::normalize() const
{
    const _Type i_len = static_cast<_Type>(1) / length();

    return *this * i_len;
}

template<vec4_type _Type>
zyra_always_inline constexpr _Type vec4_basic<_Type>::dot(const vec4_basic<_Type>& v) const
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type> vec4_basic<_Type>::to_rad() const
{
    return *this * static_cast<_Type>(math::constants::k_d2r);
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type> vec4_basic<_Type>::to_deg() const
{
    return *this * static_cast<_Type>(math::constants::k_r2d);
}

template<vec4_type _Type>
zyra_always_inline constexpr bool vec4_basic<_Type>::is_zero() const
{
    return *this == kNull;
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type> vec4_basic<_Type>::lerp(const vec4_basic<_Type>& v, const _Type t) const
{
    return *this + (v - *this) * t;
}

template<vec4_type _Type>
zyra_always_inline constexpr vec4_basic<_Type> vec4_basic<_Type>::cross(const vec4_basic<_Type>& v1, const vec4_basic<_Type>& v2, const vec4_basic<_Type>& v3)
{
    return {
        v1.y * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.y * v3.w - v3.y * v2.w) + v1.w * (v2.y * v3.z - v2.z * v3.y),
        -(v1.x * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.z - v3.x * v2.z)),
        v1.x * (v2.y * v3.w - v3.y * v2.w) - v1.y * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.y - v3.x * v2.y),
        -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x * v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y))
    };
}

zyra_end_
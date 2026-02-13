#pragma once
#include <zyra/util/def.h>
#include <cstdint>
#include <cstddef>
#include <cstring>


zyra_begin_

typedef std::uint32_t murmur2_hash_t;
typedef std::uint64_t murmur2_hash64_t;

namespace murmur2 {
#undef _to_lower_u
#define _to_lower_u(c) (std::uint32_t)(((c >= 'A') && (c <= 'Z')) ? c + 32 : c)
    template <bool LowerCase = true>
    [[nodiscard]] inline murmur2_hash_t hash_a1(const char* text, std::uint32_t seed) {
        std::size_t length = std::strlen(text);
        std::size_t N = length + 1;

        const std::uint32_t m = 0x5bd1e995u;
        const int r = 24;

        std::uint32_t h = seed ^ static_cast<std::uint32_t>((N - 1)); // exclude null terminator

        std::size_t i = 0;
        while (i + 4 <= N - 1) {
            std::uint32_t k;
            if constexpr (LowerCase) {
                k =
                    _to_lower_u(text[i]) |
                    (_to_lower_u(text[i + 1]) << 8) |
                    (_to_lower_u(text[i + 2]) << 16) |
                    (_to_lower_u(text[i + 3]) << 24);
            }
            else {
                k =
                    (std::uint32_t)text[i] |
                    ((std::uint32_t)text[i + 1] << 8) |
                    ((std::uint32_t)text[i + 2] << 16) |
                    ((std::uint32_t)text[i + 3] << 24);
            }

            k *= m;
            k ^= k >> r;
            k *= m;

            h *= m;
            h ^= k;

            i += 4;
        }

        if constexpr (LowerCase) {
            switch ((N - 1) - i) {
            case 3: h ^= _to_lower_u(text[i + 2]) << 16; [[fallthrough]];
            case 2: h ^= _to_lower_u(text[i + 1]) << 8; [[fallthrough]];
            case 1: h ^= _to_lower_u(text[i]); h *= m;
            }
        }
        else {
            switch ((N - 1) - i) {
            case 3: h ^= (std::uint32_t)text[i + 2] << 16; [[fallthrough]];
            case 2: h ^= (std::uint32_t)text[i + 1] << 8; [[fallthrough]];
            case 1: h ^= (std::uint32_t)text[i]; h *= m;
            }
        }

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }

    template <bool LowerCase = true, std::size_t N>
    [[nodiscard]] consteval murmur2_hash_t hash_a1_const(const char(&text)[N], std::uint32_t seed) {
        const std::uint32_t m = 0x5bd1e995u;
        const int r = 24;

        std::uint32_t h = seed ^ (N - 1); // exclude null terminator

        std::size_t i = 0;
        while (i + 4 <= N - 1) {
            std::uint32_t k;
            if constexpr (LowerCase) {
                k =
                    _to_lower_u(text[i]) |
                    (_to_lower_u(text[i + 1]) << 8) |
                    (_to_lower_u(text[i + 2]) << 16) |
                    (_to_lower_u(text[i + 3]) << 24);
            }
            else {
                k =
                    (std::uint32_t)text[i] |
                    ((std::uint32_t)text[i + 1] << 8) |
                    ((std::uint32_t)text[i + 2] << 16) |
                    ((std::uint32_t)text[i + 3] << 24);
            }

            k *= m;
            k ^= k >> r;
            k *= m;

            h *= m;
            h ^= k;

            i += 4;
        }

        if constexpr (LowerCase) {
            switch ((N - 1) - i) {
            case 3: h ^= _to_lower_u(text[i + 2]) << 16; [[fallthrough]];
            case 2: h ^= _to_lower_u(text[i + 1]) << 8; [[fallthrough]];
            case 1: h ^= _to_lower_u(text[i]); h *= m;
            }
        }
        else {
            switch ((N - 1) - i) {
            case 3: h ^= (std::uint32_t)text[i + 2] << 16; [[fallthrough]];
            case 2: h ^= (std::uint32_t)text[i + 1] << 8; [[fallthrough]];
            case 1: h ^= (std::uint32_t)text[i]; h *= m;
            }
        }

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }

    [[nodiscard]] inline murmur2_hash64_t hash_64_b(const char* text, std::size_t len, std::uint64_t seed) {
        const std::uint32_t m = 0x5BD1E995u;
        const int r = 24;

        std::uint32_t h1 = static_cast<std::uint32_t>(seed ^ len);
        std::uint32_t h2 = static_cast<std::uint32_t>(seed >> 32);

        std::size_t i = 0u;

        while (i + 8u <= len) {
            std::uint32_t k1 =
                (std::uint32_t)(unsigned char)(text[i]) |
                ((std::uint32_t)(unsigned char)(text[i + 1]) << 8) |
                ((std::uint32_t)(unsigned char)(text[i + 2]) << 16) |
                ((std::uint32_t)(unsigned char)(text[i + 3]) << 24);
            i += 4u;

            k1 *= m; k1 ^= (k1 >> r); k1 *= m;
            h1 *= m; h1 ^= k1;

            std::uint32_t k2 =
                (std::uint32_t)(unsigned char)(text[i]) |
                ((std::uint32_t)(unsigned char)(text[i + 1]) << 8) |
                ((std::uint32_t)(unsigned char)(text[i + 2]) << 16) |
                ((std::uint32_t)(unsigned char)(text[i + 3]) << 24);
            i += 4u;

            k2 *= m; k2 ^= (k2 >> r); k2 *= m;
            h2 *= m; h2 ^= k2;
        }

        if (i + 4u <= len) {
            std::uint32_t k1 =
                (std::uint32_t)(unsigned char)(text[i]) |
                ((std::uint32_t)(unsigned char)(text[i + 1]) << 8) |
                ((std::uint32_t)(unsigned char)(text[i + 2]) << 16) |
                ((std::uint32_t)(unsigned char)(text[i + 3]) << 24);
            i += 4;

            k1 *= m; k1 ^= (k1 >> r); k1 *= m;
            h1 *= m; h1 ^= k1;
        }

        switch (len - i) {
        case 3: h2 ^= (std::uint32_t)(unsigned char)text[i + 2] << 16; [[fallthrough]];
        case 2: h2 ^= (std::uint32_t)(unsigned char)text[i + 1] << 8; [[fallthrough]];
        case 1: h2 ^= (std::uint32_t)(unsigned char)text[i]; h2 *= m;
        }

        h1 ^= (h2 >> 18); h1 *= m;
        h2 ^= (h1 >> 22); h2 *= m;
        h1 ^= (h2 >> 17); h1 *= m;
        h2 ^= (h1 >> 19); h2 *= m;

        return (static_cast<std::uint64_t>(h1) << 32) | h2;
    }

    template <std::size_t N>
    [[nodiscard]] consteval murmur2_hash64_t hash_64_b_const(const char(&text)[N], std::uint64_t seed) {
        const std::uint32_t m = 0x5BD1E995u;
        const int r = 24;

        std::uint32_t len = N - 1;

        std::uint32_t h1 = static_cast<std::uint32_t>(seed) ^ len;
        std::uint32_t h2 = static_cast<std::uint32_t>(seed >> 32);

        std::size_t i = 0;

        while (i + 8 <= len) {
            std::uint32_t k1 =
                (std::uint32_t)(unsigned char)(text[i]) |
                ((std::uint32_t)(unsigned char)(text[i + 1]) << 8) |
                ((std::uint32_t)(unsigned char)(text[i + 2]) << 16) |
                ((std::uint32_t)(unsigned char)(text[i + 3]) << 24);
            i += 4;

            k1 *= m; k1 ^= (k1 >> r); k1 *= m;
            h1 *= m; h1 ^= k1;

            std::uint32_t k2 =
                (std::uint32_t)(unsigned char)(text[i]) |
                ((std::uint32_t)(unsigned char)(text[i + 1]) << 8) |
                ((std::uint32_t)(unsigned char)(text[i + 2]) << 16) |
                ((std::uint32_t)(unsigned char)(text[i + 3]) << 24);
            i += 4;

            k2 *= m; k2 ^= (k2 >> r); k2 *= m;
            h2 *= m; h2 ^= k2;
        }

        if (i + 4 <= len) {
            std::uint32_t k1 =
                (std::uint32_t)(unsigned char)(text[i]) |
                ((std::uint32_t)(unsigned char)(text[i + 1]) << 8) |
                ((std::uint32_t)(unsigned char)(text[i + 2]) << 16) |
                ((std::uint32_t)(unsigned char)(text[i + 3]) << 24);
            i += 4;

            k1 *= m; k1 ^= (k1 >> r); k1 *= m;
            h1 *= m; h1 ^= k1;
        }

        switch ((N - 1) - i) {
        case 3: h2 ^= (std::uint32_t)(unsigned char)text[i + 2] << 16; [[fallthrough]];
        case 2: h2 ^= (std::uint32_t)(unsigned char)text[i + 1] << 8; [[fallthrough]];
        case 1: h2 ^= (std::uint32_t)(unsigned char)text[i]; h2 *= m;
        }

        h1 ^= (h2 >> 18); h1 *= m;
        h2 ^= (h1 >> 22); h2 *= m;
        h1 ^= (h2 >> 17); h1 *= m;
        h2 ^= (h1 >> 19); h2 *= m;

        return (static_cast<uint64_t>(h1) << 32) | h2;
    }
}

zyra_end_
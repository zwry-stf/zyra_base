#pragma once
#include <zyra/util/def.h>
#include <cstdint>
#include <cstddef>
#include <cstring>


zyra_begin_

namespace fnv1a {
    using fnv1a_hash_t = std::uint64_t;

    [[nodiscard]] consteval fnv1a_hash_t hash_const(const char* string, const std::uint64_t key = 0xCBF29CE484222325ULL) {
        return (string[0] == '\0') ? key : hash_const(
            &string[1],
            (key ^ static_cast<std::uint64_t>(string[0])) * 0x100000001B3ULL
        );
    }

    [[nodiscard]] inline fnv1a_hash_t hash(const char* string, std::uint64_t key = 0xCBF29CE484222325ULL) {
        const auto length = std::strlen(string);

        for (std::size_t i = 0U; i < length; ++i) {
            key ^= string[i];
            key *= 0x100000001B3ULL;
        }

        return key;
    }

    consteval fnv1a_hash_t fnv1a_64(const char* s) noexcept {
        fnv1a_hash_t h = 14695981039346656037ull;
        for (std::size_t i = 0; s[i] != '\0'; ++i) {
            h ^= static_cast<unsigned char>(s[i]);
            h *= 0x100000001B3ULL;
        }
        return h;
    }

    consteval fnv1a_hash_t splitmix64_next(fnv1a_hash_t& x) noexcept {
        x += 0x9E3779B97F4A7C15ull;
        fnv1a_hash_t z = x;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
        return z ^ (z >> 31);
    }
}

zyra_end_
#pragma once
#include <zyra/util/murmur2.h>
#if not defined(ZYRA_PUBLIC)
#include <string_view>
#endif // ZYRA_PUBLIC


zyra_begin_

class string_token {
private:
    murmur2_hash_t hash_;

#if not defined(ZYRA_PUBLIC)
    std::string_view debug_name_;
#endif // ZYRA_PUBLIC

public:
    constexpr string_token()
        : hash_(0u)
#if not defined(ZYRA_PUBLIC)
        , debug_name_()
#endif
    {
    }

    template <size_t N>
    consteval string_token(const char(&text)[N])
        : hash_(murmur2::hash_a1_const<false>(text, 0x85931285u))
#if not defined(ZYRA_PUBLIC)
        , debug_name_(text, text + N - 1u)
#endif // ZYRA_PUBLIC
    { 
    }

public:
#if not defined(ZYRA_PUBLIC)
    [[nodiscard]] const auto& get_debug_name() const noexcept {
        return debug_name_;
    }
#endif // ZYRA_PUBLIC

public:
    [[nodiscard]] bool operator==(const string_token& v) const noexcept { 
        return hash_ == v.hash_;
    }
    [[nodiscard]] bool operator!=(const string_token& v) const noexcept {
        return !(*this == v); 
    }
};

zyra_end_
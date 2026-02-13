#pragma once
#include <zyra/util/xstr.h>
#include <zyra/util/string_token.h>
#include <format>


#if not defined(ZYRA_PUBLIC)
template<>
struct std::formatter<zyra::string_token> : std::formatter<std::string_view> {
    auto format(const zyra::string_token& value, auto& ctx) const {
        return std::formatter<std::string_view>::format(value.get_debug_name(), ctx);
    }
};
#endif


template<>
struct std::formatter<zyra::xstr> : std::formatter<std::string_view> {
    auto format(const zyra::xstr& value, auto& ctx) const {
        char buffer[value.kMaxLength];
        value.get(buffer);
        return std::formatter<std::string_view>::format(
            std::string_view(buffer, value.length()), ctx);
    }
};

template<>
struct std::formatter<zyra::basic_xstr<char, 128u>> : std::formatter<std::string_view> {
    auto format(const zyra::basic_xstr<char, 128u>& value, auto& ctx) const {
        char buffer[value.kMaxLength];
        value.get(buffer);
        return std::formatter<std::string_view>::format(
            std::string_view(buffer, value.length()), ctx);
    }
};

template<>
struct std::formatter<zyra::basic_xstr<char, 256u>> : std::formatter<std::string_view> {
    auto format(const zyra::basic_xstr<char, 256u>& value, auto& ctx) const {
        char buffer[value.kMaxLength];
        value.get(buffer);
        return std::formatter<std::string_view>::format(
            std::string_view(buffer, value.length()), ctx);
    }
};
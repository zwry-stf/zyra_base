#pragma once
#include <cstdint>
#include <cstddef>
#include <type_traits>


zyra_begin_

template <std::size_t Count>
class signature_parser {
protected:
    std::int16_t bytes_[Count]{};
    std::uint32_t length_;
    std::int16_t key_;
    std::uint32_t abs_offset_;

protected:
    [[nodiscard]] static consteval std::int16_t generate_key() {
        std::int16_t key = 0u;
        std::uint32_t counter = 0x8255;
        while (key == 0u) {
            key = static_cast<std::int16_t>(
                (__TIME__[1] + __TIME__[4] | 
                (__TIME__[6] ^ __TIME__[7])) ^ counter
            );
            counter++;
        }
        return key;
    }

public:
    template <std::size_t N>
    consteval signature_parser(const char(&text)[N])
        : length_(0u), 
          key_(generate_key()), 
          abs_offset_(0u) {
        parse(text);
    }

    consteval signature_parser()
        : length_(0),
          key_(generate_key()),
          abs_offset_(0)
    {
    }

    [[nodiscard]] static signature_parser<Count> from_bytes(std::uint8_t* bytes, std::size_t l) {
        signature_parser<Count> ret;
        for (std::size_t i = 0u; i < l; i++)
            ret.bytes_[ret.length_++] = static_cast<std::int16_t>(bytes[i]) ^ ret.key_;

        return ret;
    }

public:
    [[nodiscard]] std::uint32_t length() const noexcept { 
        return length_; 
    }

    [[nodiscard]] constexpr std::int16_t operator[](std::uint32_t i) const noexcept {
        return bytes_[i] ^ key_;
    }

    [[nodiscard]] std::uint32_t abs_offset() const noexcept {
        return abs_offset_;
    }

protected:
    [[nodiscard]] consteval std::int16_t char_to_byte(char c) {
        if (c >= '0' && c <= '9')
            return static_cast<std::int16_t>(c - '0');
        else if (c >= 'a' && c <= 'f')
            return static_cast<std::int16_t>(c - 'a' + 10);
        else if (c >= 'A' && c <= 'F')
            return static_cast<std::int16_t>(c - 'A' + 10);
        else
            throw "invalid char";
    }

    [[nodiscard]] consteval std::int16_t parse_byte(char* buf, std::uint32_t length) {
        if (length == 2u &&
            buf[0] == '{' &&
            buf[1] == '}') {
            if (abs_offset_ != 0)
                throw "more than one abs wildmark found";

            abs_offset_ = length_;

            return -2;
        }
        std::int16_t res = 0;

        bool has_wildmark = false;
        for (std::uint32_t i = 0u; i < length; i++) {
            auto c = buf[i];
            if (has_wildmark &&
                c != '?')
                throw "invalid wild mark";

            if (c == '?') {
                has_wildmark = true;
            }
            else {
                res *= 0x10;
                res += char_to_byte(c);
            }
        }

        return has_wildmark ? -1 : res;
    }

    template <std::size_t N>
    consteval void parse(const char(&text)[N]) {
        constexpr std::uint32_t kMaxCharPerByte = 2;

        std::uint32_t num_curr_byte = 0;

        char curr_byte_buf[kMaxCharPerByte];

        for (std::size_t i = 0u; i < N; i++) {
            if (text[i] == ' ' ||
                text[i] == '\0') {
                if (num_curr_byte > 0) {
                    auto ret = parse_byte(curr_byte_buf, num_curr_byte);
                    if (ret == -2) {
                        if (length_ + 4u >= N)
                            throw "signature too long";

                        bytes_[length_++] = -1 ^ key_;
                        bytes_[length_++] = -1 ^ key_;
                        bytes_[length_++] = -1 ^ key_;
                        bytes_[length_++] = -1 ^ key_;
                    }
                    else {
                        if (length_ + 1 >= N)
                            throw "signature too long";

                        bytes_[length_++] = ret ^ key_;
                    }
                }

                num_curr_byte = 0;
                continue;
            }

            if (num_curr_byte == kMaxCharPerByte)
                throw "byte to long";

            curr_byte_buf[num_curr_byte++] = text[i];
        }

        if (length_ == 0u)
            throw "invalid signature";

        // cut trailing wild cards
        while ((*this)[length_ - 1u] == -1) {
            if (length_ == 1u)
                throw "invalid signature";

            length_--;
        }

        if (length_ == 0u)
            throw "invalid signature";
    }
};

using default_signature = signature_parser<128u>;

zyra_end_
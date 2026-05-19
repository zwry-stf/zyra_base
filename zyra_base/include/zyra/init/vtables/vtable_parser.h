#pragma once
#include <type_traits>
#include <zyra/init/signatures/signature_parser.h>


zyra_begin_

template <std::size_t Count>
class vtable_parser : public signature_parser<Count> {
public:
    template <size_t N>
    consteval vtable_parser(const char(&text)[N])
        : signature_parser<Count>() {
        parse(text);
    }

private:
    template <std::size_t N>
    consteval void parse(const char(&text)[N]) {
        constexpr char kTypeDescriptorStart[] = ".?AV";
        constexpr char kTypeDescriptorEnd[] = "@@";

        static_assert(
            (N + sizeof(kTypeDescriptorStart) - 1 + sizeof(kTypeDescriptorEnd) - 1) <= Count,
            "vtable string to long"
        );

        for (size_t i = 0; i < sizeof(kTypeDescriptorStart) - 1; i++)
            this->bytes_[this->length_++] = static_cast<std::int16_t>(kTypeDescriptorStart[i]) ^ this->key_;

        for (size_t i = 0; i < N - 1; i++)
            this->bytes_[this->length_++] = static_cast<std::int16_t>(text[i]) ^ this->key_;

        for (size_t i = 0; i < sizeof(kTypeDescriptorEnd) - 1; i++)
            this->bytes_[this->length_++] = static_cast<std::int16_t>(kTypeDescriptorEnd[i]) ^ this->key_;
        this->bytes_[this->length_++] = static_cast<std::int16_t>('\0') ^ this->key_;
    }
};

using default_vtable = vtable_parser<128u>;

zyra_end_
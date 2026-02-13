#pragma once
#include <zyra/util/fnv1a.h>
#include <cstddef>
#include <concepts>
#if not defined(assert)
#include <assert.h>
#endif


zyra_begin_

template <typename T>
concept xstr_character =
    std::same_as<T, char> ||
    std::same_as<T, signed char> ||
    std::same_as<T, unsigned char> ||
    std::same_as<T, wchar_t> ||
    std::same_as<T, char8_t> ||
    std::same_as<T, char16_t> ||
    std::same_as<T, char32_t>;

template <xstr_character Char, std::size_t MaxLength>
class basic_xstr {
private:
    std::size_t length_;
    Char key_;
    Char chars_[MaxLength];

    inline static constexpr Char kNullTerminator = Char(0);

    template<xstr_character, std::size_t>
    friend class basic_xstr;
    friend class iterator;

public:
    inline static constexpr std::size_t kMaxLength = MaxLength;

public:
    // helpers
    [[nodiscard]] consteval static Char generate_key_const(std::uint64_t counter) noexcept {
        fnv1a::fnv1a_hash_t s = 0;

        auto c = static_cast<fnv1a::fnv1a_hash_t>(counter);
        s ^= fnv1a::splitmix64_next(c);

        s ^= fnv1a::fnv1a_64(__FILE__);
        s ^= fnv1a::fnv1a_64(__DATE__);
        s ^= fnv1a::fnv1a_64(__TIME__);
        s ^= (static_cast<fnv1a::fnv1a_hash_t>(__LINE__) << 32);
        s ^= static_cast<fnv1a::fnv1a_hash_t>(__COUNTER__) * 0xD6E8FEB86659FD93ull;

        const auto r = fnv1a::splitmix64_next(s);
        return static_cast<Char>(r);
    }

private:
    consteval void init_bytes_random() {
        for (std::size_t i = 0; i < MaxLength; i++) {
            chars_[i] = generate_key_const(static_cast<std::uint64_t>(i));
        }
    }

    [[nodiscard]] zyra_always_inline constexpr static Char encrypt(Char c, Char k) noexcept {
        return c ^ k;
    }
    [[nodiscard]] zyra_always_inline constexpr static Char decrypt(Char c, Char k) noexcept {
        return c ^ k;
    }

public:
    consteval basic_xstr() noexcept
        : length_(0u),
          key_(generate_key_const(0u)),
          chars_() {
        init_bytes_random();
    }

    template <std::size_t N>
        requires (N - 1 <= MaxLength && N > 0u)
    consteval basic_xstr(const Char(&text)[N]) noexcept
        : basic_xstr() {
        length_ = N - 1u;

        for (std::size_t i = 0u; i < length_; i++) {
            chars_[i] = encrypt(text[i], key_);
        }
    }

    constexpr basic_xstr(const basic_xstr<Char, MaxLength>&) = default;
    constexpr basic_xstr(basic_xstr<Char, MaxLength>&&) = default;
    constexpr basic_xstr<Char, MaxLength>& operator=(const basic_xstr<Char, MaxLength>&) = default;
    constexpr basic_xstr<Char, MaxLength>& operator=(basic_xstr<Char, MaxLength>&&) = default;

    template <std::size_t N>
    zyra_always_inline basic_xstr<Char, MaxLength>& operator=(const basic_xstr<Char, N>& v) noexcept {
        if (reinterpret_cast<const void*>(this) == reinterpret_cast<const void*>(&v))
            return *this;

        assert(v.length_ <= MaxLength);

        key_ = v.key_;
        length_ = v.length_;

        for (std::size_t i = 0u; i < length_; i++)
            chars_[i] = v.chars_[i];

        return *this;
    }

public:
    class iterator {
    private:
        std::size_t pos_;
        basic_xstr<Char, MaxLength>* const ref_;

    public:
        zyra_always_inline iterator(std::size_t pos, basic_xstr<Char, MaxLength>* ref) noexcept
            : pos_(pos),
              ref_(ref) { }

    public:
        [[nodiscard]] zyra_always_inline Char operator*() const noexcept {
            return (*ref_)[pos_];
        }
        [[nodiscard]] zyra_always_inline explicit operator Char() const noexcept {
            return (*ref_)[pos_];
        }
        zyra_always_inline iterator& operator++() noexcept {
            assert(pos_ < MaxLength);
            pos_++;
            return *this;
        }
        zyra_always_inline iterator& operator--() noexcept {
            assert(pos_ > 0u);
            pos_--;
            return *this;
        }
        zyra_always_inline iterator& operator=(Char v) noexcept {
            ref_->chars_[pos_] = encrypt(v, ref_->key_);
        }
    };

public:
    template <std::size_t N>
    zyra_always_inline constexpr basic_xstr<Char, MaxLength>& operator+=(const basic_xstr<Char, N>& other) {
        assert(length_ + other.length_ <= MaxLength);

        for (std::size_t i = 0u; i < other.length_; i++)
            chars_[length_ + i] = encrypt(other[i], key_);

        length_ += other.length_;

        return *this;
    }

    template <std::size_t N>
    [[nodiscard]] zyra_always_inline constexpr basic_xstr<Char, MaxLength> operator+(const basic_xstr<Char, N>& other) const {
        basic_xstr ret = *this;
        ret += other;

        return ret;
    }

    zyra_always_inline constexpr basic_xstr<Char, MaxLength>& operator+=(Char c) {
        assert(length_ + 1u <= MaxLength);

        chars_[length_++] = encrypt(c, key_);

        return *this;
    }

    [[nodiscard]] zyra_always_inline constexpr basic_xstr<Char, MaxLength> operator+(Char c) const {
        basic_xstr ret = *this;
        ret += c;

        return ret;
    }

public:
    void append(const Char* str) {
        while (*str != kNullTerminator) {
            assert(length_ + 1u <= MaxLength);
            chars_[length_++] = encrypt(*str, key_);
            ++str;
        }
    }

    template <std::size_t N>
    void append(const basic_xstr<Char, N>& other) {
        assert(length_ + other.length_ <= MaxLength);

        for (std::size_t i = 0u;
             i < other.length_; i++) {
            (*this) += other[i];
        }
    }

    void append_safe(const Char* str) {
        while (*str != kNullTerminator) {
            if (length_ == MaxLength)
                return;

            chars_[length_++] = encrypt(*str, key_);
            ++str;
        }
    }

    template <std::size_t N>
    void append_safe(const basic_xstr<Char, N>& other) {
        std::size_t length = other.length_;
        if (length_ + length > MaxLength) {
            length = MaxLength - length_;
        }

        for (std::size_t i = 0u;
             i < length; i++) {
            (*this) += other[i];
        }
    }

    [[nodiscard]] zyra_always_inline bool equals(const Char* other) const noexcept {
        for (std::size_t i = 0u; i < length_; i++) {
            Char c = decrypt(chars_[i], key_);
            if (c != other[i])
                return false;
        }

        if (other[length_] != kNullTerminator)
            return false;

        return true;
    }
    template <std::size_t N>
    [[nodiscard]] zyra_always_inline bool equals(const basic_xstr<Char, N>& other) const noexcept {
        if (length_ != other.length_)
            return false;

        for (std::size_t i = 0u; i < length_; i++) {
            if ((*this)[i] != other[i])
                return false;
        }

        return true;
    }

public:
    template <std::integral T>
    [[nodiscard]] zyra_always_inline iterator operator[](T index) noexcept {
        return iterator(static_cast<std::size_t>(index), this);
    }
    template <std::integral T>
    [[nodiscard]] zyra_always_inline Char operator[](T index) const noexcept {
        return chars_[index] ^ key_;
    }
    template <std::integral T>
    [[nodiscard]] zyra_always_inline Char at(T index) const noexcept {
        return (*this)[index];
    }

    [[nodiscard]] zyra_always_inline std::size_t length() const noexcept {
        return length_;
    }
    [[nodiscard]] zyra_always_inline bool empty() const noexcept {
        return length_ == 0u;
    }

    [[nodiscard]] zyra_always_inline bool operator==(const Char* other) const {
        return equals(other);
    }
    template <std::size_t N>
    [[nodiscard]] zyra_always_inline bool operator==(const basic_xstr<Char, N>& other) const {
        return equals(other);
    }

public:
    template <std::size_t N>
    void get(Char(&out)[N]) const noexcept {
        get(&out[0], N * sizeof(Char));
    }

    void get(Char* out, std::size_t size) const noexcept {
        if (size == 0u)
            return;

        std::size_t len2 = (size / sizeof(Char)) - 1u;
        std::size_t len = length_ > len2 ? len2 : length_;

        for (std::size_t i = 0u; i < len; i++)
            out[i] = chars_[i] ^ key_;

        out[len] = kNullTerminator;
    }
};

using xstr = basic_xstr<char, 64>;

zyra_end_
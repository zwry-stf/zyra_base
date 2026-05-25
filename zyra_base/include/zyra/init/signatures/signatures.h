#pragma once
#include <zyra/util/class.h>
#include <zyra/util/access.h>
#include <zyra/util/xstr.h>
#include <zyra/util/string_token.h>
#include "signature_parser.h"

#include <vector>
#include <functional>
#include <type_traits>


zyra_begin_

struct replacable_signature_t {
public:
    inline static std::uint64_t constexpr k_magic_value = 0x7439a69243bf2962ull;

    std::uint64_t magic{ k_magic_value };
    bool is_patched{ false };
    union {
        default_signature raw_signature;
        std::uintptr_t pointer;
    };

    template <std::size_t N>
    consteval replacable_signature_t(const char(&sig)[N])
        : raw_signature(sig) { }
};

class c_signatures {
    zyra_remove_copy_constructor(c_signatures);
    zyra_remove_move_constructor(c_signatures);

private:
    struct loaded_signature_t {
        void* address;
        string_token hash;
    };
    std::vector<loaded_signature_t> signatures_;
    bool failed_;

    friend class c_hooks;

public:
    c_signatures() noexcept = default;

#ifndef ZYRA_PUBLIC
    using signature_t = default_signature;
#else
    using signature_t = replacable_signature_t;
#endif // ZYRA_PUBLIC

public:
    /*
        usage:
        g_signatures()->initialize_layer(
            [](auto&& add) {
                add("Name", "module.dll", "E8 {} ? AB");
            }
        );
    */
    template <class Fn>
    [[nodiscard]] bool initialize_layer(Fn&& fn) {
        failed_ = false;
        fn([this](const string_token& name, const xstr& module, const signature_t& signature) {
            this->add(name, module, signature);
            }
        );
        return !failed_;
    }

    template <typename T>
        requires (std::is_pointer_v<T>)
    [[nodiscard]] zyra_always_inline T get(const string_token& hash) const {
        return reinterpret_cast<T>(get_impl(hash));
    }

    template <typename T>
        requires (std::is_pointer_v<T>)
    [[nodiscard]] zyra_always_inline T try_get(const string_token& hash) const {
        return reinterpret_cast<T>(get_impl(hash, true));
    }

    void add(const string_token& name, const xstr& module, const signature_t& signature);

public:
    [[nodiscard]] void* find_signature(const xstr& module, const default_signature& signature);
    [[nodiscard]] void* find_signature(void* start, void* end, const default_signature& signature, bool debug = false);

private:
    [[nodiscard]] void* get_impl(const string_token& hash, bool do_try = false) const;
};

zyra_define_access(c_signatures, g_signatures);


#define zyra_get_signature_ex(_var, _name, _type) \
static auto _var = ::zyra::g_signatures()->get<_type>(_name)

#define zyra_get_signature(_var, _name, _ret, _args) \
zyra_get_signature_ex(_var, _name, _ret(*)_args)

zyra_end_
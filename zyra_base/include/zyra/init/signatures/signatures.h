#pragma once
#include <zyra/util/class.h>
#include <zyra/util/access.h>
#include <zyra/util/xstr.h>
#include <zyra/util/string_token.h>
#include "signature_parser.h"
#include <vector>
#include <functional>


zyra_begin_

class c_signatures {
    zyra_remove_copy_constructor(c_signatures);
    zyra_remove_move_constructor(c_signatures);

private:
    struct loaded_signature_t {
        void* address;
        string_token hash;
    };
    std::vector<loaded_signature_t> signatures_;

    friend class c_hooks;

#if defined(_DEBUG)
    bool failed_;
#endif // _DEBUG

public:
    c_signatures();
    ~c_signatures() = default;

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
    void initialize_layer(Fn&& fn) {
        fn([this](const string_token& name, const xstr& module, const default_signature& signature) {
            this->add(name, module, signature);
            }
        );
        on_init();
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

public:
    [[nodiscard]] void* find_signature(const xstr& module, const default_signature& signature);
    [[nodiscard]] void* find_signature(void* start, void* end, const default_signature& signature, bool debug = false);

private:
    [[nodiscard]] void* get_impl(const string_token& hash, bool do_try = false) const;
    void on_init();

private:
    void add(const string_token& name, const xstr& module, const default_signature& signature);
};

zyra_define_access(c_signatures, g_signatures);


#define zyra_get_signature_ex(_var, _name, _type) \
static auto _var = g_signatures()->get<_type>(_name)

#define zyra_get_signature(_var, _name, _ret, _args) \
zyra_get_signature_ex(_var, _name, _ret(*)_args)

zyra_end_
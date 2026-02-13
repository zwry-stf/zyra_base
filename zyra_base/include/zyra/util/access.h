#pragma once
#include <zyra/util/def.h>
#include <utility>


template <typename Type>
requires (std::is_default_constructible_v<Type>)
class c_access {
private:
    Type value_;

public:
    c_access() = default;
    ~c_access() = default;

    c_access(const c_access&) = delete;
    c_access& operator=(const c_access&) = delete;
    c_access(c_access<Type>&&) = delete;
    c_access& operator=(c_access<Type>&&) = delete;

public:
    [[nodiscard]] zyra_always_inline Type* operator()() noexcept {
        return &value_; 
    }
    [[nodiscard]] zyra_always_inline const Type* operator()() const noexcept {
        return &value_;
    }
};

#define zyra_define_access(_class, _name) \
inline c_access<_class> _name
#pragma once
#include <zyra/util/def.h>
#include <utility>


template <typename Type>
    requires (std::is_default_constructible_v<Type>)
class c_access {
private:
    static Type& get_instance() noexcept {
        static Type instance;
        return instance;
    }

public:
    c_access() {
        static int instance_count = 0;
        if (instance_count != 0)
            __debugbreak(); // only one instance per type should exist
        instance_count++;
    }
    ~c_access() = default;
    c_access(const c_access&) = delete;
    c_access& operator=(const c_access&) = delete;
    c_access(c_access<Type>&&) = delete;
    c_access& operator=(c_access<Type>&&) = delete;

public:
    [[nodiscard]] zyra_always_inline Type* operator()() noexcept {
        return &get_instance();
    }
    [[nodiscard]] zyra_always_inline const Type* operator()() const noexcept {
        return &get_instance();
    }
};

#define zyra_define_access(_class, _name) \
inline c_access<_class> _name
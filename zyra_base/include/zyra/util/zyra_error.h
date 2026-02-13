#pragma once
#include <zyra/util/xstr.h>
#include <cstdlib>


zyra_begin_

class fast_fail {
public:
    fast_fail() {
        std::abort();
    }
};

class zyra_error {
public:
    zyra_error(const xstr& err) {
        char buffer[xstr::kMaxLength];
        err.get(buffer);

        throw_error(buffer);
    }

private:
    void throw_error(const char* err);
};

zyra_end_
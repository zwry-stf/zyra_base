#pragma once
#include <zyra/util/def.h>
#include <cmath>


zyra_begin_

namespace math {
    namespace constants {
        inline constexpr float k_pi = 3.14159265358979323846f;
        inline constexpr float k_2_pi = k_pi * 2.f;
        inline constexpr float k_pi_div_2 = k_pi / 2.f;
        inline constexpr float k_1_div_2pi = 1.f / k_2_pi;
        inline constexpr float k_d2r = k_pi / 180.f;
        inline constexpr float k_r2d = 180.f / k_pi;
    }
}

zyra_end_
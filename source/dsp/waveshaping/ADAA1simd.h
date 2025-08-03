#pragma once

#include <cstdlib>

#include "../SIMD.h"

namespace Electrophilia::Waveshaper
{
    const vec4 TOL = 1.0e-8;

    class ADAA1simd
    {
    public:

        void reset()
        {
            x1 = 0.0;
            ad1_x1 = 0.0;
        }

        inline vec4 process (vec4 x) noexcept
        {
            auto illCondition = vec4::lessThan(vec4::abs(x - x1), TOL);
            vec4 ad1_x = nonlinearityAntiderivative (x);

            vec4 branch1 = (ad1_x - ad1_x1) / (x - x1);
            vec4 branch2 = nonlinearity ((x + x1) * 0.5f);

            vec4 y = (branch2 & illCondition) + ( branch1 & ~illCondition);

            ad1_x1 = ad1_x;
            x1 = x;

            return branch2;
        }

    protected:
        virtual inline vec4 nonlinearity (vec4 x) const noexcept = 0;
        virtual inline vec4 nonlinearityAntiderivative (vec4 x) const noexcept = 0;

        vec4 x1 = 0.0;
        vec4 ad1_x1 = 0.0;
    };
}

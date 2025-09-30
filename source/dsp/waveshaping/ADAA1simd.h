#pragma once

#include <cstdlib>

#include "../../math/Simd.h"

namespace Ath::Waveshaper
{
    class ADAA1simd
    {
    public:
        const Simd::float8 TOL = 1.0e-8;

        void reset()
        {
            x1 = 0.0;
            ad1_x1 = 0.0;
        }

        inline Simd::float8 process (Simd::float8 x) noexcept
        {
            Simd::float8 ad1_x = nonlinearityAntiderivative (x);

            Simd::float8 branch1 = (ad1_x - ad1_x1) / (x - x1);
            Simd::float8 branch2 = nonlinearity ((x + x1) * 0.5f);

            Simd::float8 y = Simd::ternary(branch2, branch1, Simd::abs(x - x1) < TOL);

            ad1_x1 = ad1_x;
            x1 = x;

            return y;
        }

    protected:
        virtual inline Simd::float8 nonlinearity (Simd::float8 x) const noexcept = 0;
        virtual inline Simd::float8 nonlinearityAntiderivative (Simd::float8 x) const noexcept = 0;

        Simd::float8 x1 = 0.0;
        Simd::float8 ad1_x1 = 0.0;
    };
}

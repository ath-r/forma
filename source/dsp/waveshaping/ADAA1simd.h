#pragma once

#include <cstdlib>

#include "../../math/Simd.h"

namespace Ath::Dsp::Waveshaper
{
    template<typename T>
    class ADAA1simd
    {
    public:
        const T TOL = 1.0e-8;

        void reset()
        {
            x1 = 0.0;
            ad1_x1 = 0.0;
        }

        inline T process (T x) noexcept
        {
            T ad1_x = nonlinearityAntiderivative (x);

            T branch1 = (ad1_x - ad1_x1) / (x - x1);
            T branch2 = nonlinearity ((x + x1) * 0.5f);

            T y = Simd::ternary(branch2, branch1, Simd::abs(x - x1) < TOL);

            ad1_x1 = ad1_x;
            x1 = x;

            return y;
        }

    protected:
        virtual inline T nonlinearity (T x) const noexcept = 0;
        virtual inline T nonlinearityAntiderivative (T x) const noexcept = 0;

        T x1 = 0.0;
        T ad1_x1 = 0.0;
    };
}

#pragma once

#include "dsp/waveshaping/ADAA1simd.h"
#include "math/Polynomial.h"

namespace Ath::Waveshaper
{
    class FormaFilterStageNonlinearity : public ADAA1simd
    {
        Math::Polynomial<vec4, 14> polynomial =
        {
            {
                1.71728130e-06, -6.81356896e-06, -4.60207337e-05,  2.09700640e-04,
                3.98066854e-04, -2.51414230e-03, -3.41678719e-04,  1.43025324e-02,
                -1.51068577e-02, -3.05161787e-02,  1.00311937e-01, -1.87138284e-01,
                -9.98138618e-01, -4.59153383e-16
            }
        };

        Math::Polynomial<vec4, 15> antiderivative = polynomial.antiderivative();

        inline vec4 nonlinearity (vec4 x) const noexcept override
        {
            return polynomial.evaluate(x);
        }

        inline vec4 nonlinearityAntiderivative (vec4 x) const noexcept override
        {
            return antiderivative.evaluate(x);
        }
    };
}

#pragma once

#include "dsp/waveshaping/ADAA1simd.h"
#include "math/Polynomial.h"
#include "math/Simd.h"

namespace Ath::Forma
{
    class FormaFilterNonlinearity : public Waveshaper::ADAA1simd
    {
        Math::Polynomial<Simd::float8, 14> polynomial =
        {
            {
                1.71728130e-06f, -6.81356896e-06f, -4.60207337e-05f,  2.09700640e-04f,
                3.98066854e-04f, -2.51414230e-03f, -3.41678719e-04f,  1.43025324e-02f,
                -1.51068577e-02f, -3.05161787e-02f,  1.00311937e-01f, -1.87138284e-01f,
                -9.98138618e-01f, -4.59153383e-16f
            }
        };

        Math::Polynomial<Simd::float8, 15> antiderivative = polynomial.antiderivative();

        inline Simd::float8 nonlinearity (Simd::float8 x) const noexcept override
        {
            return polynomial.evaluate(x);
        }

        inline Simd::float8 nonlinearityAntiderivative (Simd::float8 x) const noexcept override
        {
            return antiderivative.evaluate(x);
        }
    };
}

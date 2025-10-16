#pragma once

#include "dsp/waveshaping/ADAA1.h"
#include "dsp/waveshaping/ADAA1simd.h"
#include "math/Polynomial.h"
#include "math/Simd.h"

namespace Ath::Forma
{
    class FormaTransistorNonlinearity : public Dsp::Waveshaper::ADAA1
    {
        Math::Polynomial<double, 14> polynomial =
        {
            {
                -2.14079872e-06,  8.49393729e-06,  5.73704072e-05, -2.61417194e-04,
                -4.96238449e-04,  3.13418227e-03,  4.25943823e-04, -1.78298354e-02,
                1.88325242e-02,  3.80421054e-02, -1.25050955e-01,  2.33290491e-01,
                1.24430044e+00,  1.31125927e-16
            }
        };

        Math::Polynomial<double, 15> antiderivative = polynomial.antiderivative();

        inline double nonlinearity (double x) const noexcept override
        {
            return polynomial.evaluate(x);
        }

        inline double nonlinearityAntiderivative (double x) const noexcept override
        {
            return antiderivative.evaluate(x);
        }
    };

    class FormaTransistorNonlinearity8 : public Dsp::Waveshaper::ADAA1simd<Simd::float8>
    {
        Math::Polynomial<Simd::float8, 14> polynomial =
        {
            {
                -2.14079872e-06f,  8.49393729e-06f,  5.73704072e-05f, -2.61417194e-04f,
                -4.96238449e-04f,  3.13418227e-03f,  4.25943823e-04f, -1.78298354e-02f,
                1.88325242e-02f,  3.80421054e-02f, -1.25050955e-01f,  2.33290491e-01f,
                1.24430044e+00f,  1.31125927e-16f
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

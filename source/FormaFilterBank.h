#pragma once

#include <array>
#include "dsp/Filter.h"
#include "math/Complex.h"
#include "math/Simd.h"
#include "dsp/Context.h"

#include "FormaBiquadCoefficients.h"

namespace Ath::Forma 
{
    class FormaFilterBank
    {
        static constexpr int INPUT_NUM = 6;
        static constexpr int CASCADE_SIZE = 8;
        Dsp::Context c;

    public:
        std::array<Dsp::Filter::Biquad::BiquadCascade<Simd::float8, CASCADE_SIZE>, INPUT_NUM> cascades;
    
        void setContext(Dsp::Context context)
        {
            c = context;

            for (int i = 0; i < INPUT_NUM; i++) // inputs
            {
                for (int j = 0; j < CASCADE_SIZE; j++) // inners
                {
                    auto c16 = Dsp::Filter::Biquad::bilinear(BIQUAD_COEFFS[i][j], c.SR);
                    auto c8 = Dsp::Filter::Biquad::bilinear(BIQUAD_COEFFS[i + INPUT_NUM][j], c.SR);
                    auto c4 = Dsp::Filter::Biquad::bilinear(BIQUAD_COEFFS[i + INPUT_NUM * 2][j], c.SR);
                    auto c2 = Dsp::Filter::Biquad::bilinear(BIQUAD_COEFFS[i + INPUT_NUM * 3][j], c.SR);
                    auto c5 = Dsp::Filter::Biquad::bilinear(BIQUAD_COEFFS[i + INPUT_NUM * 4][j], c.SR);
                    auto c1 = Dsp::Filter::Biquad::bilinear(BIQUAD_COEFFS[i + INPUT_NUM * 5][j], c.SR);

                    Simd::float8 b0 = {float(c16.b0), float(c8.b0), float(c4.b0), float(c2.b0), float(c5.b0), float(c1.b0), 1.0f, 1.0f};
                    Simd::float8 b1 = {float(c16.b1), float(c8.b1), float(c4.b1), float(c2.b1), float(c5.b1), float(c1.b1), 0.0f, 0.0f};
                    Simd::float8 b2 = {float(c16.b2), float(c8.b2), float(c4.b2), float(c2.b2), float(c5.b2), float(c1.b2), 0.0f, 0.0f};
                    Simd::float8 a0 = {float(c16.a0), float(c8.a0), float(c4.a0), float(c2.a0), float(c5.a0), float(c1.a0), 1.0f, 1.0f};
                    Simd::float8 a1 = {float(c16.a1), float(c8.a1), float(c4.a1), float(c2.a1), float(c5.a1), float(c1.a1), 0.0f, 0.0f};
                    Simd::float8 a2 = {float(c16.a2), float(c8.a2), float(c4.a2), float(c2.a2), float(c5.a2), float(c1.a2), 0.0f, 0.0f};

                    cascades[i].biquads[j].setCoefficients({b0, b1, b2, a0, a1, a2} );

                    cascades[i].biquads[j].reset();
                }
            }
        }

        Math::complex<Simd::float8> getTransfer(Simd::float8 frequency, int index)
        {
            Math::complex<Simd::float8> transfer = { 1.0f, 0.0f }; // PLACEHOLDER

            for (auto& biquad : cascades[index].biquads)
            {
                transfer *= Dsp::Filter::Biquad::transfer(biquad.coeffs, Dsp::Filter::f2s(frequency), Simd::float8(c.SR));
            }

            return transfer;
        }
    };
}
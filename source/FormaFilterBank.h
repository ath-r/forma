#pragma once

#include <algorithm>
#include <array>
#include "math/Complex.h"
#include "math/Conversion.h"
#include "math/Simd.h"
#include "dsp/Context.h"
#include "dsp/filter/FilterMath.h"

namespace Ath::Forma 
{
    class FormaFilterBank
    {
        static constexpr int STAGE_NUM = 6;

        int stages = 6;

        Dsp::Context c;
        Simd::float8 frequency = 30.0f;

        std::array<Dsp::Filter::LowPass1<Simd::float8>, STAGE_NUM> filters;
        Dsp::Filter::HighPass1<Simd::float8> highpass;

        public:
        Simd::float8 hmul = { 1.0, 2.0, 4.0, 8.0, 3.0, 10.0, 1.0, 1.0 };
        std::array<Simd::float8, STAGE_NUM> vmul = { 1.0f, 2.828f, 4.0f, 5.656f, 8.0, 11.313f};
        
        void setContext(Dsp::Context context)
        {
            c = context;

            for (auto& filter : filters) filter.setContext(c);
            highpass.setContext(c);

            setFrequency(frequency);
        }

        void setStageNumber(int n)
        {
            stages = std::clamp(n, 0, STAGE_NUM);
        }

        void setFrequency(Simd::float8 freq)
        {
            frequency = freq;

            for (int i = 0; i < STAGE_NUM; i++)
            {
                auto& filter = filters[i];

                filter.setCutoffFrequency(frequency * hmul * vmul[i]);
            }
            highpass.setCutoffFrequency(freq * hmul);
        }

        Simd::float8 getAttenutation(Simd::float8 frequency)
        {
            Math::complex<Simd::float8> transfer = { 1.0f, 0.0f };

            for (int i = 0; i < stages; i++)
            {
                transfer *= filters[i].getTransfer(frequency);
            }

            return Simd::rmag(transfer.re, transfer.im);
        }

        Simd::float8 process(Simd::float8 x)
        {
            x = highpass.process(x);
            auto y = x;

            for (int i = 0; i < stages; i++)
            {
                y = filters[i].process(y);
            }
            return y + x * Math::DB_MINUS48;
        }


    };
}
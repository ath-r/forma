#pragma once

#include <algorithm>
#include <array>
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

        public:
        Simd::float8 hmul = { 1.0, 2.0, 4.0, 8.0, 3.0, 10.0, 1.0, 1.0 };
        std::array<Simd::float8, STAGE_NUM> vmul = { 1.0f, 2.828f, 4.0f, 5.656f, 8.0, 11.313f};
        
        void setContext(Dsp::Context context)
        {
            c = context;

            for (auto& filter : filters) filter.setContext(c);
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
        }

        Simd::float8 process(Simd::float8 x)
        {
            for (int i = 0; i < stages; i++)
            {
                x = filters[i].process(x);
            }
            return x;
        }


    };
}
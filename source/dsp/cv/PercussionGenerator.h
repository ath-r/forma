#pragma once

#include "../Context.h"
#include "../../math/Conversion.h"
#include <algorithm>

namespace Ath::Dsp::Cv
{
    template <typename T>
    class PercussionGenerator
    {
        Context c;
        T time = 1.0;

        T lastInput = 0.0;
        T a = 0.9;
        T y = 0.0;
public:
        void setContext(Context context)
        {
            c = context;
            setTime(time);
        }

        void setTime(T t)
        {
            time = std::clamp(t, 0.001, 10.0);
            auto decayInSamples = t * c.SR;
            a = std::pow(0.01, 1.0 / decayInSamples);
        }

        T process(T gateIn)
        {
            T gateTruncated = (gateIn > 0.5f) ? 1.0f : 0.0f;

            if (gateTruncated != lastInput) y = gateTruncated;
            lastInput = gateTruncated;

            y *= a;
            return y;
        }
    };
}
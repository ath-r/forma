#pragma once

#include "Context.h"
#include "../math/Trigonometry.h"

namespace Electrophilia::Dsp
{
    template <typename T>
    class Filter1P
    {
        Context c = Context(48000.0f);
        T G = 0.0f;
        T z1 = 0.0f;
        T y;

    public:

        void reset()
        {
            z1 = 0;
        }

        void setContext(const Context context)
        {
            c = context;
        }

        void setCutoffFrequencyPrewarped(T freq)
        {
            const T wd = freq * 6.28318531f;
            const T wa = Math::fastTan(wd * c.T * 0.5f) * (2.0f * c.SR);
            const T g = wa * c.T * 0.5f;

            G = g / (g + 1.0f);
        }

        void setCutoffFrequency(T freq)
        {
            G = c.T * freq * 0.5f;
        }

        void setTime(T time)
        {
            G = 0.5f / (c.SR * time);
        }

        inline T process(T x)
        {
            const T v = (x - z1) * G;
            y = v + z1;
            z1 = v + y;
            return y;
        }

        inline T last()
        {
            return y;
        }
    };
}

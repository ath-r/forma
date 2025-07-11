#pragma once

#include "Context.h"
#include "../math/Trigonometry.h"
#include "SIMD.h"

namespace Electrophilia::Dsp
{
    template <typename T>
    class Filter1P
    {
        Context c = Context(48000.0f);
        T G = 0.0f;
        T z1 = 0.0f;
        T y;

        T frequency;

    public:

        void reset()
        {
            z1 = 0;
        }

        void setContext(const Context context)
        {
            c = context;

            setCutoffFrequency(frequency);
        }

        void setCutoffFrequencyPrewarped(T freq)
        {
            frequency = freq;

            const T wd = freq * Math::tau<T>;
            const T wa = Math::fastTan(wd * T(c.T) * T(0.5)) * (T(c.SR * 2.0));
            const T g = wa * T(c.T * 0.5);

            G = g / (g + 1.0f);
        }

        void setCutoffFrequency(T freq)
        {
            frequency = freq;

            const T g = freq * T(c.T) * Math::fpi;
            G = g / (g + 1.0f);
        }

        void setTime(T time)
        {
            G = T(0.5) / (time * T(c.SR));
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

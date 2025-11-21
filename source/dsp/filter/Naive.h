#pragma once

#include "../Context.h"
#include "FilterMath.h"

namespace Ath::Dsp::Filter::Naive
{
    template <typename T>
    static inline T processLP(T x, T& y, T g)
    {
        y += (x - y) * g;
        return y;
    }

    template <typename T>
    class LowPass1
    {
        Context c;
        T g = 0.0f;
        T y = 0.0f;

        T frequency = 100.0f;

    public:
        void reset()
        {
            y = 0;
        }

        void setContext(const Context context)
        {
            c = context;
            setCutoffFrequency(frequency);
        }

        void setCutoffFrequency(T freq)
        {
            frequency = freq;
            g = frequencyToG(freq, T(c.T));
        }

        virtual inline T process(T x)
        {
            y = processLP(x, y, g);
            return y;
        }

        inline T last()
        {
            return y;
        }
    };
}
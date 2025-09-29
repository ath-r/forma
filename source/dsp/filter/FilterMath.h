#pragma once

#include "../Context.h"
#include "../../math/Trigonometry.h"
#include "../../math/Complex.h"

namespace Ath::Dsp::Filter
{
    template <typename T>
    static inline T normFrequencyToG(T freq)
    {
        const T g = freq * Math::fpi;
        return g / (g + T(1.0f));
    }

    template <typename T>
    static inline T frequencyToG(T freq, T sampleT)
    {
        return normFrequencyToG(freq * sampleT);
    }

    template <typename T>
    static inline T timeToG(T time, T sampleT)
    {
        const T freq = T(0.5f) * sampleT / time;
        return normFrequencyToG(freq);
    }

    template <typename T>
    static inline T processLP(T x, T& z1, T G)
    {
        const T v = (x - z1) * G;
        const T y = v + z1;
        z1 = v + y;
        return y;
    }

    template <typename T>
    static inline T processHP(T x, T& z1, T G)
    {
        return x - processLP(x, z1, G);
    }

    template <typename T>
    static inline T processHP(T x, T& z1, T G, T g2)
    {
        const T y = (x - z1) * G;
        z1 = z1 + y * g2;
        return y;
    }

    using namespace Math;
    template<typename T>
    static complex<T> transferLP1(complex<T> wc, complex<T> s)
    {
        return wc / (wc + s);
    }


    template <typename T>
    class LowPass1
    {
        Context c;
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

        void setCutoffFrequency(T freq)
        {
            frequency = freq;

            G = frequencyToG(freq, T(c.T));
        }

        Math::complex<T> getTransfer(T freq)
        {
            Math::complex<T> wc = { frequency * Math::ftau, 0.0f };
            Math::complex<T> s = { 0.0f, freq * Math::ftau };

            return transferLP1(wc, s);
        }

        inline T process(T x)
        {
            y = processLP(x, z1, G);
            return y;
        }

        inline T last()
        {
            return y;
        }
    };
}
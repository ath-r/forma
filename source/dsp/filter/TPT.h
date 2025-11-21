#pragma once

#include "FilterMath.h"
#include "../Context.h"
#include "../../math/Trigonometry.h"
#include "../../math/Complex.h"

namespace Ath::Dsp::Filter::TPT 
{
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

    template <typename T>
    class LowPass1
    {
    private:
        Context c;
        T G = 0.0f;
        T z1 = 0.0f;
        T y = 0.0f;

        T frequency = 100.f;

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

        virtual inline T process(T x)
        {
            y = processLP(x, z1, G);
            return y;
        }

        inline T last()
        {
            return y;
        }
    };

    template <typename T>
    class HighPass1
    {
        Context c;
        T G = 0.0f;
        T z1 = 0.0f;
        T y = 0.0f;

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

            return transferHP1(wc, s);
        }

        inline T process(T x)
        {
            y = processHP(x, z1, G);
            return y;
        }
    };
}
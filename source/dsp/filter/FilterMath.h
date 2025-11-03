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

    template<typename T>
    static Math::complex<T> transferLP1(Math::complex<T> wc, Math::complex<T> s)
    {
        return wc / (wc + s);
    }

    template<typename T>
    static Math::complex<T> transferHP1(Math::complex<T> wc, Math::complex<T> s)
    {
        return s / (wc + s);
    }
}
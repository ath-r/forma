#include "SquareChebyshev8.h"
#include "../../math/Trigonometry.h"

namespace Ath::Dsp::Oscillator
{
    void SquareChebyshev8::setContext (const Context context)
    {
        c = context;

        setFrequency (frequency);
    }

    void SquareChebyshev8::setFrequency (const float8 newFrequency)
    {
        frequency = newFrequency;

        delta = frequency * c.T;

        n = Math::truncate (15000.0f / frequency[0]);
        n = Math::max(n, 1.0f);
    }

    void SquareChebyshev8::setTime (const double time)
    {
        auto t = frequency * float(time);

        phase = Simd::mod1f(t);
    }

    float8 SquareChebyshev8::processSample()
    {
        phase += delta;
        phase = Simd::mod1f(phase);

        const float8 x = sin2pi9(phase);
        const float8 x2 = x * x;
        const float8 x3 = x2 * x;

        const float8 x21 = x2 * 4.0f - 2.0f;

        float8 cheb_iminus2 = x;
        float8 cheb_i = x3 * 4.0f - x * 3.0f;
        float8 cheb_iplus2;

        float8 sum = cheb_iminus2 - cheb_i * 0.333333333f;

        constexpr float falloff_start_frequency = 14500.0;
        constexpr float falloff_end_frequency = 15000.0;
        constexpr float falloff_a = -1.0f / (falloff_end_frequency - falloff_start_frequency);
        constexpr float falloff_b = 1.0f - falloff_start_frequency * falloff_a;

        const float8 falloff_freq = frequency * falloff_a;
        float8 falloff;

        const float8 mult[4] = {0.0f, 1.0f, 0.0f, -1.0f};
        
        float8 i_f = 5.0f;
        for (int i = 5; i < n; i += 2)
        {
            cheb_iplus2 = Simd::fma(x21, cheb_i, -cheb_iminus2);

            cheb_iminus2 = cheb_i;
            cheb_i = cheb_iplus2;

            falloff = Simd::max(Simd::min(fma(falloff_freq, i_f, falloff_b), 1.0f), 0.0f);

            sum += cheb_i * Simd::recip(i_f) * mult[i % 4];
            i_f += 2.0f;
        }

        return sum;
        
    }
}
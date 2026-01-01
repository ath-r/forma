#include "SquareChebyshev8.h"
#include "../../math/Math.h"

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

        // This algorithm computes harmonics of the square wave with Chebyshev polynomials
        // Traditionally, Chebyshev polynomials are defined by this recurrence relation:
        // T_0(x) = 1
        // T_1(x) = x
        // T_{n+1}(x) = 2xT_n(x) - T_{n-1}(x)

        // Because square waves only contain odd harmonics, this relation may be redefined
        // to only compute odd-order polynomials
        // T_1(x) = x
        // T_3(x) = 4x^3 - 3x
        // T_{n+2}(x) = T_n(x)(x^2 * 4 - 2) - T_{n-4}(x) where n>=5 and is odd

        float8 cheb_iminus2 = x;
        float8 cheb_i = x3 * 4.0f - x * 3.0f;
        float8 cheb_iplus2;
        const float8 x21 = x2 * 4.0f - 2.0f;

        // n is the total number of harmonics to compute
        // n = max_harmonics / frequency[0]
        // frequency[1...8] will be a multiple of frequency[0]
        // which means that aliasing will occur if harmonics are not otherwise attenuated 

        // harmonics will roll off smoothly between falloff_start_frequency and falloff_end_frequency
        constexpr float falloff_start_frequency = 14500.0f;
        constexpr float falloff_end_frequency = 15000.0f;
        constexpr float falloff_a = -1.0f / (falloff_end_frequency - falloff_start_frequency);
        constexpr float falloff_b = 1.0f - falloff_start_frequency * falloff_a;

        const float8 falloff_freq = frequency * falloff_a;
        float8 falloff;

        // overy other harmonic's phase must be inverted
        const float8 mult[4] = {0.0f, 1.0f, 0.0f, -1.0f};

        // manually add first and third harmonics multiplied by falloff
        float8 sum = cheb_iminus2 * Simd::clamp(falloff_freq + falloff_b, 0.0f, 1.0f);
        sum += cheb_i * 0.333333333f * Simd::clamp(Simd::fma(falloff_freq, 3.0f, falloff_b), 0.0f, 1.0f);
        
        float8 i_f = 5.0f;
        for (int i = 5; i < n; i += 2)
        {
            cheb_iplus2 = Simd::fma(x21, cheb_i, -cheb_iminus2);

            cheb_iminus2 = cheb_i;
            cheb_i = cheb_iplus2;

            falloff = Simd::clamp(Simd::fma(falloff_freq, i_f, falloff_b), 0.0f, 1.0f);

            sum += cheb_i * Simd::recip(i_f) * falloff * mult[i % 4] ;
            i_f += 2.0f;
        }

        y = sum;
        return y;     

    }
}
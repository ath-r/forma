#include "SquareSincIntegral8.h"
#include "../../math/Clamp.h"
#include "../../math/Trigonometry.h"
#include "SquareChebyshev8.h"

namespace Ath::Dsp::Oscillator
{
    void SquareSincIntegral8::setContext (const Context context)
    {
        c = context;

        setFrequency (frequency);
    }

    void SquareSincIntegral8::setFrequency (const float8 newFrequency)
    {
        frequency = newFrequency;
        delta = frequency * c.T;   

        n = Simd::floor(float8(7500.0f) / frequency);
        n = Simd::max(n, float8(1.0f));
    }

    void SquareSincIntegral8::setTime (const double time)
    {
        const auto t = frequency * float(time);

        phase = Simd::mod1f(t);
    }

    float8 SquareSincIntegral8::processSample()
    {
        phase += delta;
        phase = Simd::mod1f(phase);

        auto centeredPhase = phase - 0.5f;
        auto triangularPhase = (float8(0.5f) - Simd::abs(centeredPhase)) * 2.0f - 0.5f;

        y = sincIntegral(triangularPhase * n);
        return y;
    }

    float8 SquareSincIntegral8::sincIntegral (float8 x)
    {
        static const float8 _corr = 1.0f + (1.0f / Math::fpi);
        static const auto _inv_corr = float8(1.0f) / _corr;

        const auto f0 = sin2pi9(x * 0.5f) * _corr;

        const auto invx = float8(1.0f) / (x * Math::ftau);
        const auto invx2 = invx * invx;
        const auto f1 = Simd::sign(x) - sin2pi9(x + 0.25f) * invx - sin2pi9(x) * invx2;

        const auto mask = (x < 0.5f) & (x > -0.5f);
        const auto si = Simd::ternary(f0, f1, mask);

        return si * _inv_corr;

    }
}

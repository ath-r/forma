#include "SquareSincIntegral.h"
#include "../../math/Clamp.h"
#include "../../math/Trigonometry.h"

namespace Ath::Dsp::Oscillator
{
    void SquareSincIntegral::setContext (const Context context)
    {
        c = context;

        setFrequency (frequency);
    }

    void SquareSincIntegral::setFrequency (const float newFrequency)
    {
        frequency = newFrequency;

        delta = frequency * c.T;

        n = Math::truncate (7500.0f / frequency);
        n = Math::max(n, 1.0f);
    }

    void SquareSincIntegral::setTime (const double time)
    {
        const float t = float(time) * frequency;

        phase = t - Math::truncate (t);
    }

    float SquareSincIntegral::processSample()
    {
        phase += delta;
        phase = Math::fastmod1f(phase);

        const float centeredPhase = phase - 0.5f;
        const float triangularPhase = (0.5f - std::abs(centeredPhase)) * 2.0f - 0.5f;

        return sincIntegral(triangularPhase * n);
    }

    float SquareSincIntegral::sincIntegral (float x)
    {
        constexpr float pi1 = 1.0f + (1.0f / Math::fpi);

        const float sin05x = Math::sin2pi9(x * 0.5f);
        const float f0 = sin05x * pi1;

        const float invx = 1.0f / (x * Math::ftau);
        const float invx2 = invx * invx;
        const float sin1x = Math::chebyshev2(sin05x);
        const float f1 = Math::sign(x) - (Math::sin2pi9(x + 0.25f) * invx + sin1x * invx2);

        const auto condition = (x < 0.5f) & (x > -0.5f);
        const float si = condition ? f0 : f1;

        return si / pi1;
    }
}

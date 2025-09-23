#include "SquareNaive.h"
#include "../../math/Trigonometry.h"

namespace Ath::Dsp::Oscillator
{
    void SquareNaive::setContext (const Context context)
    {
        c = context;

        setFrequency (frequency);
    }

    void SquareNaive::setFrequency (const float newFrequency)
    {
        frequency = newFrequency;

        delta = frequency * c.T;
    }

    void SquareNaive::setTime (const double time)
    {
        const float t = float(time) * frequency;

        phase = Math::fastmod1f(t);
    }

    float SquareNaive::processSample()
    {
        phase += delta;
        phase = Math::fastmod1f(phase);

        return phase > 0.5f ? 1.0f : -1.0f;
    }
}
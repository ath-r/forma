#pragma once

#include "Context.h"
#include "../math/Clamp.h"
#include "../math/Trigonometry.h"

#include "Filter.h"

namespace Electrophilia::Dsp::Oscillator
{
    class BlitSquare
    {
        Context c = Context(48000.0f);
        float n;

        float phase = 0.0;
        float frequency = 0.0f;
        float delta = 0.0f;

        Filter1P<float> filter;

        float out = 0.0f;
        float lastOut = 0.0f;

public:
        void setContext(const Context context)
        {
            c = context;
            filter.setContext(c);

            setFrequency(frequency);
        }

        void setFrequency(const float newFrequency)
        {
            frequency = newFrequency;
            filter.setCutoffFrequency(10.0);

            delta = frequency * c.T;

            n = std::floor(18000.0f / frequency);
        }

        float processSample()
        {
            phase += delta;
            phase = Math::fastmod1f(phase);

            float core = (dirichlet(phase) - dirichlet(phase + 0.5f)) / (2.0f * n);

            out = core * delta + lastOut * (1.0f - delta * 0.1f);
            lastOut = out;

            return lastOut;
        }

private:

        float dirichlet(float x)
        {
            constexpr float threshold = 1e-4;

            float nominator = Math::sin2pi9((n + 0.5f) * x);
            float denominator = Math::sin2pi9(x * 0.5f);

            float fallback = 2.0f * n - 1.0f;

            return ((std::abs(denominator) < threshold) ? fallback : (nominator / denominator));
        }
    };
}

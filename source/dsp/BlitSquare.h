#pragma once

#include "Context.h"
#include "../math/Math.h"

#include "Filter.h"
#include "SIMD.h"

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


    #include "SIMD.h"

    class BlitSquareSimd
    {
        Context c = Context(48000.0f);
        vec4 n;

        vec4 phase = 0.0;
        vec4 frequency = 0.0f;
        vec4 delta = 0.0f;

        vec4 out = 0.0f;
        vec4 lastOut = 0.0f;

    public:
        void setContext(const Context context)
        {
            c = context;

            setFrequency(frequency);
        }

        void setFrequency(const vec4 newFrequency)
        {
            frequency = newFrequency;

            delta = frequency * c.T;

            n = vec4::truncate(18000.0f / frequency);
        }

        vec4 processSample()
        {
            phase += delta;
            phase -= vec4::truncate(phase);

            vec4 core = (dirichlet(phase) - dirichlet(phase + 0.5f)) / (n * 2.0f);

            out = core * delta + lastOut * (vec4(1.0f) - delta);
            lastOut = out;

            return out * 128.0f;
        }

    private:
        vec4 foldArgument(vec4 x)
        {
            const vec4 half = vec4(0.5f);
            x = x - vec4::truncate(x);

            return vec4::max(vec4::min(x, half - x), vec4(0.0f) - half - x);

        }

        vec4 sin2pi9(vec4 x)
        {
            const vec4 x1 = foldArgument(x);
            const vec4 x2 = x1 * x1;

            const vec4 a = 6.28318516008947744301885339855754539f;
            const vec4 b = -41.3416550314162780771649724741397745f;
            const vec4 c = 81.6010040732617735242889484141942461f;
            const vec4 d = -76.5497822935957426856648840708956891f;
            const vec4 e = 39.5367060657302079906898367421553316f;

            return x1 * (a + x2 * (b + x2 * (c + x2 * (d + x2 * e))));
        }

        vec4 dirichlet(vec4 x)
        {
            const vec4 threshold = 1e-4;

            vec4 nominator = sin2pi9((n + 0.5f) * x);
            vec4 denominator = sin2pi9(x * 0.5f);

            vec4 fallback = n * 2.0f - 1.0f;

            auto cond = vec4::lessThan(vec4::abs(denominator), threshold);

            return (fallback & cond) + ((nominator / denominator) & ~cond);

        }
    };
}

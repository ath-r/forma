#include "BlitSquareSimd.h"

namespace Electrophilia::Dsp::Oscillator
{

    void BlitSquareSimd::setContext (const Context context)
    {
        c = context;

        setFrequency (frequency);
    }

    void BlitSquareSimd::setFrequency (const vec4 newFrequency)
    {
        frequency = newFrequency;

        delta = frequency * c.T;

        vec4 t = _time / frequency;
        phase = t - vec4::truncate (t);

        n = vec4::truncate (18000.0f / frequency);
    }

    void BlitSquareSimd::setTime (const double time)
    {
        const vec4 t = vec4 (float (time)) / frequency;

        _time = float (time);

        phase = t - vec4::truncate (t);
    }

    vec4 BlitSquareSimd::processSample()
    {
        _time += c.T;

        phase += delta;
        phase -= vec4::truncate (phase);

        vec4 core = (dirichlet (phase) - dirichlet (phase + 0.5f)) / (n * 2.0f);

        out = core * delta + lastOut * (vec4 (1.0f) - delta);
        lastOut = out;

        return out * 128.0f;
    }

    vec4 BlitSquareSimd::foldArgument (vec4 x)
    {
        const vec4 half = vec4 (0.5f);
        x = x - vec4::truncate (x);

        return vec4::max (vec4::min (x, half - x), vec4 (0.0f) - half - x);
    }

    vec4 BlitSquareSimd::sin2pi9 (vec4 x)
    {
        const vec4 x1 = foldArgument (x);
        const vec4 x2 = x1 * x1;

        const vec4 a = 6.28318516008947744301885339855754539f;
        const vec4 b = -41.3416550314162780771649724741397745f;
        const vec4 c = 81.6010040732617735242889484141942461f;
        const vec4 d = -76.5497822935957426856648840708956891f;
        const vec4 e = 39.5367060657302079906898367421553316f;

        return x1 * (a + x2 * (b + x2 * (c + x2 * (d + x2 * e))));
    }

    vec4 BlitSquareSimd::dirichlet (vec4 x)
    {
        const vec4 threshold = 1e-4;

        vec4 nominator = sin2pi9 ((n + 0.5f) * x);
        vec4 denominator = sin2pi9 (x * 0.5f);

        vec4 fallback = n * 2.0f - 1.0f;

        auto cond = vec4::lessThan (vec4::abs (denominator), threshold);

        return (fallback & cond) + ((nominator / denominator) & ~cond);
    }
}

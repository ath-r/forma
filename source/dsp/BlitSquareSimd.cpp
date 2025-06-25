#include "BlitSquareSimd.h"

namespace Electrophilia::Dsp::Oscillator
{

    void BlitSquareSimd::setContext (const Context context)
    {
        c = context;
        filter1.setContext(c);
        filter2.setContext(c);

        setFrequency (frequency);
        filter1.setCutoffFrequency(frequency);
        filter2.setCutoffFrequency(frequency * 0.5f);
    }

    void BlitSquareSimd::setFrequency (const vec4 newFrequency)
    {
        frequency = newFrequency;

        delta = frequency * c.T;

        vec4 t = _time / frequency;
        phase = t - vec4::truncate (t);

        n = vec4::truncate (15000.0f / frequency);

        filter1.setCutoffFrequency(frequency);
        filter2.setCutoffFrequency(frequency);
    }

    void BlitSquareSimd::setTime (const double time)
    {
        const vec4 t = vec4 (float (time)) / frequency;

        _time = float(time);

        phase = t - vec4::truncate (t);
    }

    vec4 BlitSquareSimd::processSample()
    {
        _time += c.T;

        phase += delta;
        phase -= vec4::truncate (phase);

        const vec4 centeredPhase = phase - 0.5f;

        const vec4 core = dirichlet(centeredPhase) - dirichlet(centeredPhase + 0.5f);

        out = core + lastOut * (vec4(1.0f) - delta);
        lastOut = out;

        const vec4 f1 = out - filter1.process(out);
        const vec4 f2 = f1 - filter2.process(f1);

        return f2;
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
        const vec4 aMask = vec4::max(vec4(1.0f) - vec4::abs(x * 10000.0f), 0.0f);
        const vec4 bMask = vec4(1.0f) - aMask;
        const vec4 n2 = n * 2.0f;

        vec4 nominator = sin2pi9 ((n + 0.5f) * x);
        vec4 denominator = sin2pi9 (x * 0.5f) * n2;

        vec4 fallback = (n2 - 1.0f) / n2;

        return fallback * aMask + (nominator / denominator) * bMask;
    }

    vec4 BlitSquareSimd::sinc (vec4 x)
    {
        const vec4 aMask = vec4::max(vec4(1.0f) - vec4::abs(x * 10.0f), 0.0f);
        const vec4 bMask = vec4(1.0f) - aMask;

        const vec4 x2 = x * x;
        const vec4 a = vec4(1.0f) - x2 * 1.6449f;
        const vec4 b = sin2pi9(x * 0.5f) / (x * Math::pi<float>);

        return a * aMask + b * bMask;
    }
}

#include "SquareSimd.h"
#include "SIMD.h"

namespace Electrophilia::Dsp::Oscillator
{

    void SquareSimd::setContext (const Context context)
    {
        c = context;

        setFrequency (frequency);
    }

    void SquareSimd::setFrequency (const vec4 newFrequency)
    {
        frequency = newFrequency;

        delta = frequency * c.T;

        setTime(_time);

        n = vec4::truncate (7500.0f / frequency);
        n = vec4::max(n, 1.0f);
    }

    void SquareSimd::setTime (const double time)
    {
        const vec4 t = vec4 (float (time)) / frequency;

        _time = float(time);

        phase = t - vec4::truncate (t);
    }

    vec4 SquareSimd::processSample()
    {
        _time += c.T;

        phase += delta;
        phase -= vec4::truncate (phase);

        const vec4 centeredPhase = phase - 0.5f;
        const vec4 triangularPhase = (vec4(0.5f) - vec4::abs(centeredPhase)) * 2.0f - 0.5f;

        return sincIntegral(triangularPhase * n);
    }

    vec4 SquareSimd::foldArgument (vec4 x)
    {
        const vec4 half = vec4 (0.5f);
        x = x - vec4::truncate (x);

        return vec4::max (vec4::min (x, half - x), vec4 (0.0f) - half - x);
    }

    vec4 SquareSimd::sin2pi9 (vec4 x)
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

    vec4 SquareSimd::dirichlet (vec4 x)
    {
        const vec4 aMask = vec4::max(vec4(1.0f) - vec4::abs(x * 1000000.0f), 0.0f);
        const vec4 bMask = vec4(1.0f) - aMask;
        const vec4 n2 = n * 2.0f;

        const vec4 nominator = sin2pi9 ((n + 0.5f) * x);
        const vec4 denominator = sin2pi9 (x * 0.5f) * n2;
        vec4 fallback = 1.0f;

        const vec4 kernel = fallback * aMask + (nominator / denominator) * bMask;

        const vec4 cosine = sin2pi9(x * 0.5f + 0.25f);
        const vec4 window = cosine * cosine;

        return vec4::min(kernel, 0.8f) * window;
    }

    vec4 SquareSimd::sinc (vec4 x)
    {
        const vec4 aMask = vec4::max(vec4(1.0f) - vec4::abs(x * 10.0f), 0.0f);
        const vec4 bMask = vec4(1.0f) - aMask;

        const vec4 x2 = x * x;
        const vec4 a = vec4(1.0f) - x2 * 1.6449f;
        const vec4 b = sin2pi9(x * 0.5f) / (x * Math::pi<float>);

        return (a * aMask + b * bMask);
    }

    vec4 SquareSimd::sincIntegral (vec4 x)
    {
        constexpr float pi1 = 1.0f + (1.0f / Math::fpi);

        const vec4 f0 = sin2pi9(x * 0.5f) * pi1;

        const vec4 invx = vec4(1.0f) / (x * Math::ftau);
        const vec4 invx2 = invx * invx;

        const auto signMask = vec4::lessThan(x, 0.0f);
        const vec4 sign = (vec4(-1.0f) & signMask) + (vec4(1.0f) & ~signMask);

        const vec4 f1 = sign - (sin2pi9(x + 0.25f) * invx + sin2pi9(x) * invx2);

        const vec4 xw = x;
        const vec4 w0 = vec4(1.0f) & vec4::lessThan(xw, 0.5f) & vec4::greaterThan(xw, -0.5f);
        const vec4 w1 = vec4(1.0f) - w0;

        const vec4 si = w0 * f0 + w1 * f1;

        return si / pi1;
    }
}

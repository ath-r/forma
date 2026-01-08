#pragma once

#include "ath_dsp/dsp/Context.h"
#include "ath_dsp/math/Simd.h"

namespace Ath::Dsp::Oscillator
{
    using float8 = Simd::float8;

    class SquareChebyshev8
    {

        Context c = Context(48000.0f);

        float n = 1.0f;

        float8 phase = 0.0;
        float8 frequency = 0.0f;
        float8 delta = 0.0f;

        float8 y = 0.0f;

    public:

        inline float8 last() { return y; }
        
        void setContext (const Context context);

        void setFrequency (const float8 newFrequency);

        void setTime (const double time);

        float8 processSample();

        static inline float8 foldArgument(float8 x)
        {
            const float8 half = 0.5f;
            x = Simd::mod1f(x + half) - half;

            return Simd::max(Simd::min(x, half - x), -half - x);
        }

        static inline float8 sin2pi9(float8 x) noexcept
        {
            const auto x1 = foldArgument(x);
            const auto x2 = x1 * x1;

            const float8 a = 6.28318516008947744301885339855754539;
            const float8 b = -41.3416550314162780771649724741397745;
            const float8 c = 81.6010040732617735242889484141942461;
            const float8 d = -76.5497822935957426856648840708956891;
            const float8 e = 39.5367060657302079906898367421553316;

            return fma(fma(fma(fma(x2, e, d), x2, c), x2, b), x2, a) * x1;
        }
    };
}
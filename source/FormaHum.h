#pragma once

#include "dsp/Context.h"
#include "dsp/filter/FilterMath.h"

#include "math/Simd.h"
#include "math/Random.h"

namespace Ath::Forma
{
    class FormaHum
    {
        Dsp::Context c = Dsp::Context(48000.0f);

        Dsp::Filter::LowPass1<Simd::float8> filter;
        Math::Random::Mersenne rng;

        float phase = 0.0f;
        float delta = 0.0f;

    public:
        void setContext (Dsp::Context context);

        Simd::float8 process();
    };
}

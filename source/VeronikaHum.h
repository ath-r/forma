#pragma once

#include "dsp/SIMD.h"
#include "dsp/Context.h"
#include "dsp/Filter.h"
#include "math/Random.h"

namespace Electrophilia::Veronika
{
    class VeronikaHum
    {
        Dsp::Context c = Dsp::Context(48000.0f);
        Dsp::Filter1P<vec4> filter;

        Math::Random::Mersenne rng;

        float phase = 0.0f;
        float delta = 0.0f;

    public:
        void setContext (Dsp::Context context);

        vec4 process();
    };
}

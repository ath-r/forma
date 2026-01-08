#pragma once

#include "ath_dsp/dsp/Context.h"
#include "ath_dsp/dsp/Filter.h"

#include "ath_dsp/math/Random.h"

namespace Ath::Forma
{
    class FormaHum
    {
        Dsp::Context c = Dsp::Context(48000.0f);

        Dsp::Filter::TPT::LowPass1<float> filter;
        Math::Random::Mersenne rng;

        float phase = 0.0f;
        float delta = 0.0f;

    public:
        void setContext (Dsp::Context context);

        float process();
    };
}

#pragma once

#include "dsp/filter/FilterMath.h"
#include "dsp/Context.h"
#include "dsp/cv/LinearSmoother.h"

#include "control/Midi.h"
#include "math/Simd.h"
#include "math/Random.h"

namespace Ath::Forma
{
    using namespace Ath::Dsp;
    using namespace Ath::Control;

    class FormaNeedleContacts
    {
        Context c = Context(48000.0f);

        Simd::float8 actionThreshold;

        const Simd::float8 minVelocityGateAttack = 0.00001f;
        const Simd::float8 maxVelocityGateAttack = 0.05f;

        
        Simd::float8 time = 0.00001f;
        Simd::float8 delta = 0.0f;
        Simd::float8 value = 0.0f;

        Simd::int8 gate = 0.0f;
        Simd::float8 inputSign1 = 0.0f;

        Filter::LowPass1<Simd::float8> filter;

    public:
        void setContext (Context context);

        void init (int keyNumber);

        bool isActive();

        void handleNoteOn (Midi::MessageNoteOn message);

        void handleNoteOff (Midi::MessageNoteOff message);

        Simd::float8 processSample(Simd::float8 x);
    };
}

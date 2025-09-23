#pragma once

#include "dsp/Filter.h"
#include "dsp/Context.h"
#include "dsp/cv/LinearSmoother.h"

#include "control/Midi.h"
#include "math/Random.h"

namespace Ath::Forma
{
    using namespace Ath::Dsp;
    using namespace Ath::Control;

    class FormaKeySwitch
    {
        Context c = Context(48000.0f);

        float actionThreshold;

        static constexpr float minVelocityGateAttack = 0.0001f;
        static constexpr float maxVelocityGateAttack = 0.1f;

        float gate = 0.0f;
        float time = 0.0f;
        float delta = 0.0f;
        float value = 0.0f;

    public:
        void setContext (Context context);

        void setThreshold (float threshold);

        bool isActive();

        void handleNoteOn (Midi::MessageNoteOn message);

        void handleNoteOff (Midi::MessageNoteOff message);

        float processSample();
    };
}

#pragma once

#include "dsp/Filter.h"
#include "dsp/SquareSimd.h"
#include "dsp/Context.h"
#include "dsp/cv/LinearSmoother.h"

#include "control/Midi.h"
#include "math/Random.h"

namespace Electrophilia::Veronika
{
    using namespace Electrophilia::Dsp;
    using namespace Electrophilia::Control;

    class VeronikaVoice
    {
        Context c = Context(48000.0f);

        int note;
        float frequency = 440.0f;
        Oscillator::SquareSimd squareOctaves;
        Oscillator::SquareSimd squareMutations;

        Math::Random::LCG rng;
        vec4 actionThresholdOctaves;
        vec4 actionThresholdMutations;

        Filter1P<vec4> actionOctaves;
        Filter1P<vec4> actionMutations;

        Cv::LinearSmoother<float> gateTimer;
        static constexpr float minVelocityGateAttack = 0.0001f;
        static constexpr float maxVelocityGateAttack = 0.1f;



        float gate = 0.0f;

    public:
        void setContext (Context context);

        void setFrequency (float f);

        void setTime (double t);

        bool isActive();

        int getNote();

        void handleNoteOn (Midi::MessageNoteOn message);

        void handleNoteOff (Midi::MessageNoteOff message);

        vec4 processSample(vec4& octaves, vec4& mutations);
    };
}

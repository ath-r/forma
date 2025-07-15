#pragma once

#include "dsp/SquareSimd.h"
#include "dsp/Context.h"
#include "dsp/cv/LinearSmoother.h"

#include "control/Midi.h"

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

        Cv::LinearSmoother<float> gateSmoother;

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

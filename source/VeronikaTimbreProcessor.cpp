#pragma once

#include "VeronikaTimbreProcessor.h"

namespace Electrophilia::Veronika
{

    void TimbreProcessor::setContext (Dsp::Context context)
    {
        c = context;

        tp1.setContext (c);
        tp1.setFrequencies (tp1freqs);
        tp1.setDryGain (tp1SquareGain);

        tp2.setContext (c);
        tp2.setFrequencies (tp2freqs);
        tp2.setDryGain (tp2SquareGain);

        tp3.setContext (c);
        tp3.setFrequencies (tp3freqs);
        tp3.setDryGain (tp3SquareGain);

        tp4.setContext (c);
        tp4.setFrequencies (tp4freqs);
        tp4.setDryGain (tp4SquareGain);

        tp5.setContext (c);
        tp5.setFrequencies (tp5freqs);
        tp5.setDryGain (tp5SquareGain);

        tp6.setContext (c);
        tp6.setFrequencies (tp6freqs);
        tp6.setDryGain (tp6SquareGain);
    }

    void TimbreProcessor::addSignal (vec4 in, int midiNote)
    {
        int relativeNote = midiNote - Math::C2_MIDI_NOTE_NUMBER;
        int input = std::clamp ((relativeNote + 6) / 12, 0, 5);

        inputs[input] += in;
    }

    vec4 TimbreProcessor::processSample()
    {
        const vec4 out1 = tp1.process (inputs[0]);
        const vec4 out2 = tp2.process (inputs[1]);
        const vec4 out3 = tp3.process (inputs[2]);
        const vec4 out4 = tp4.process (inputs[3]);
        const vec4 out5 = tp5.process (inputs[4]);
        const vec4 out6 = tp6.process (inputs[5]);

        for (auto& x : inputs)
        {
            x = 0.0f;
        }

        return (out1 + out2 + out3 + out4 + out5 + out6) * Math::decibelsToAmplitude (-18);
    }
}

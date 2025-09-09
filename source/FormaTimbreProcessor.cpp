#pragma once

#include "FormaTimbreProcessor.h"
#include "math/Conversion.h"

namespace Ath::Forma
{

    void TimbreProcessor::setContext (Dsp::Context context)
    {
        c = context;

        constexpr float squareGain = Math::DB_MINUS60;

        tp1.setContext (c);
        tp1.setFrequencies (tp1freqs);
        tp1.setDryGain (squareGain);

        tp2.setContext (c);
        tp2.setFrequencies (tp2freqs);
        tp2.setDryGain (squareGain);

        tp3.setContext (c);
        tp3.setFrequencies (tp3freqs);
        tp3.setDryGain (squareGain);

        tp4.setContext (c);
        tp4.setFrequencies (tp4freqs);
        tp4.setDryGain (squareGain);

        tp5.setContext (c);
        tp5.setFrequencies (tp5freqs);
        tp5.setDryGain (squareGain);

        tp6.setContext (c);
        tp6.setFrequencies (tp6freqs);
        tp6.setDryGain (squareGain);

        hum.setContext(c);
    }

    void TimbreProcessor::addSignal (vec4 in, int midiNote)
    {
        int relativeNote = midiNote - Math::C2_MIDI_NOTE_NUMBER;
        int input = std::clamp ((relativeNote + 6) / 12, 0, 5);

        float gain = float((relativeNote + 6) % 12) / 12.0f * 6.0f;

        inputs[input] += in * Math::decibelsToAmplitude(gain);
    }

    vec4 TimbreProcessor::processSample()
    {
        const vec4 h = hum.process() * Math::DB_MINUS50;

        const vec4 out1 = tp1.process (inputs[0]) * Math::DB_MINUS3;
        const vec4 out2 = tp2.process (inputs[1]);
        const vec4 out3 = tp3.process (inputs[2]);
        const vec4 out4 = tp4.process (inputs[3]);
        const vec4 out5 = tp5.process (inputs[4]);
        const vec4 out6 = tp6.process (inputs[5]) * Math::DB_MINUS9;

        for (auto& x : inputs)
        {
            x = 0.0f;
        }

        const vec4 sum = out1 + out2 + out3 + out4 + out5 + out6 + h;

        return nonlinearity.process(sum * 0.0625f) * 16.0f * Math::DB_MINUS18;
    }
}

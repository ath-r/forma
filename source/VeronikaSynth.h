#pragma once

#include "math/Conversion.h"

#include "processor/MidiAudioProcessor.h"

#include "control/Midi.h"
#include "control/VoiceManager.h"

#include "dsp/Context.h"
#include "dsp/SIMD.h"
#include "dsp/Filter.h"
#include "dsp/PhaseCounter.h"

#include "VeronikaTimbreProcessor.h"
#include "VeronikaVoice.h"

#include <array>

namespace Electrophilia::Veronika
{
    class VeronikaSynth : public Processor::MidiAudioProcessor
    {
        Control::VoiceManager voiceManager;

        vec4 parameterFluteStops1 = 0;
        vec4 parameterFluteStops2 = 0;

        std::array<VeronikaVoice, 16> voices;
        Dsp::PhaseCounter phaseCounter;
        TimbreProcessor timbreProcessor;

        Dsp::Filter1P<vec4> parameterFluteStops1Smoother;
        Dsp::Filter1P<vec4> parameterFluteStops2Smoother;

        const float minVolumeOfStop = Math::decibelsToAmplitude(-50);

public:
    VeronikaSynth();

    void setContext (Dsp::Context context);

    void setParameterFlute16 (float x);

    void setParameterFlute8 (float x);

    void setParameterFlute4 (float x);

    void setParameterFlute2 (float x);

    void processBlock (float* buffer, int numberOfSamples) override;

    void handleMidiEvent (Control::Midi::Message message) override;
    };
}

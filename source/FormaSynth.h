#pragma once

#include "math/Conversion.h"

#include "processor/MidiAudioProcessor.h"

#include "control/Midi.h"
#include "control/VoiceManager.h"

#include "dsp/Context.h"
#include "dsp/SIMD.h"
#include "dsp/Filter.h"
#include "dsp/PhaseCounter.h"
#include "dsp/cv/LinearSmoother.h"

#include "FormaTimbreProcessor.h"
#include "FormaVoice.h"

#include <array>

namespace Ath::Forma
{
    class FormaSynth : public Processor::MidiAudioProcessor
    {
        Control::VoiceManager voiceManager;

        vec4 parameterFluteStops1 = 0;
        vec4 parameterFluteStops2 = 0;

        std::array<FormaVoice, 16> voices;
        std::array<FormaVoice, 16> mutationVoices;
        Dsp::PhaseCounter phaseCounter;

        TimbreProcessor timbreProcessor1;
        TimbreProcessor timbreProcessor2;

        Dsp::Filter1P<vec4> parameterFluteStops1Smoother;
        Dsp::Filter1P<vec4> parameterFluteStops2Smoother;

        float gate = 0.0f;
        Dsp::Cv::LinearSmoother<float> gateSmoother;

        const float minVolumeOfStop = Math::DB_MINUS60;

public:
    FormaSynth();

    void setContext (Dsp::Context context);

    void setParameterFlute16 (float x);

    void setParameterFlute8 (float x);

    void setParameterFlute4 (float x);

    void setParameterFlute2 (float x);

    void setParameterFlute5 (float x);

    void setParameterFlute1 (float x);

    void processBlock (float* buffer, int numberOfSamples) override;

    void handleMidiEvent (Control::Midi::Message message) override;
    };
}

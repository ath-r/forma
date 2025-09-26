#pragma once

#include <array>

#include "FormaKeySwitch.h"
#include "dsp/oscillator/SquareChebyshev8.h"
#include "processor/MidiAudioProcessor.h"

#include "control/Midi.h"

#include "dsp/Context.h"
#include "dsp/PhaseCounter.h"
#include "dsp/oscillator/SquareSincIntegral8.h"
#include "dsp/cv/LinearSmoother.h"

#include "math/Conversion.h"

namespace Ath::Forma
{
    class FormaSynth : public Processor::MidiAudioProcessor
    {
        Dsp::PhaseCounter phaseCounter;

        static constexpr int KEY_NUMBER = 61;
        static constexpr int OSC_NUMBER = 12;
        static constexpr int OSC_OUTPUTS_NUMBER = KEY_NUMBER + 12;

        std::array<Dsp::Oscillator::SquareSincIntegral8, OSC_NUMBER> oscillators;
        std::array<Dsp::Oscillator::SquareChebyshev8, OSC_NUMBER> oscillators2;

        std::array<Simd::float8, OSC_OUTPUTS_NUMBER> oscillatorOutputs;
        
        std::array<FormaKeySwitch, KEY_NUMBER> keyswitches; 
        std::array<Simd::float8, KEY_NUMBER> keyswitchInputs;
        std::array<Simd::float8, KEY_NUMBER> keyswitchOutputs;

        std::array<Dsp::Cv::LinearSmoother<float>, 6> parameterSmootherFluteStops;

        std::array<float, 8> parameterFluteStopsInputs;
        Simd::float8 parameterFluteStops;

        float gate = 0.0f;
        Dsp::Cv::LinearSmoother<float> gateSmoother;

        const float minVolumeOfStop = Math::DB_MINUS60;

public:
    FormaSynth();

    void setContext (Dsp::Context context);

    void processBlock (float* buffer, int numberOfSamples) override;

    void handleMidiEvent (Control::Midi::Message message) override;

    void setParameterFlute16 (float x);

    void setParameterFlute8 (float x);

    void setParameterFlute4 (float x);

    void setParameterFlute2 (float x);

    void setParameterFlute5 (float x);

    void setParameterFlute1 (float x);
    };
}

#pragma once

#include <array>

#include "FormaKeySwitch.h"
#include "FormaFilterBank.h"
#include "dsp/oscillator/SquareChebyshev8.h"
#include "math/Simd.h"
#include "processor/MidiAudioProcessor.h"

#include "PluginParameters.h"

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

        // Tuning error (in cents) as described in MM5555/MM5556 Chromatic Frequency Generators datasheet
        // Error is normalized based on the average
        std::array<float, 12> oscillatorTuningError = 
        {
            -0.44408f,
            -0.15808f,
            -0.16008f,
            0.24592f,
            0.38892f,
            -0.13508f,
            0.53992f,
            -0.20008f,
            -0.33908f,
            -0.11808f,
            0.17692f,
            0.20292f
        };

        std::array<Simd::float8, OSC_OUTPUTS_NUMBER> oscillatorOutputs;
        
        std::array<FormaKeySwitch, KEY_NUMBER> keyswitches; 
        std::array<Simd::float8, KEY_NUMBER> keyswitchInputs;
        std::array<Simd::float8, KEY_NUMBER> keyswitchOutputs;
        std::array<Simd::float8, KEY_NUMBER> prefilterGains;

        std::array<Dsp::Cv::LinearSmoother<float>, 6> parameterSmootherFluteStops;

        std::array<Simd::float8, 6> filterBankInputs;
        std::array<FormaFilterBank, 6> filterBanks;

        std::array<float, 8> parameterFluteStopsInputs;
        Simd::float8 parameterFluteStops;

        float gate = 0.0f;
        Dsp::Cv::LinearSmoother<float> gateSmoother;

        const float minVolumeOfStop = Math::DB_MINUS60;

        std::array<float, PARAM_COUNT> parameters;



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

    float getParameter(int id) {return parameters[id]; }
    };
}

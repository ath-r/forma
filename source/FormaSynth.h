#pragma once

#include <array>
#include <cstddef>

#include "processor/MidiAudioProcessor.h"

#include "FormaKeySwitch.h"
#include "FormaFilterBank.h"
#include "FormaTransistorNonlinearity.h"
#include "FormaHum.h"
#include "PluginParameters.h"

#include "math/Simd.h"
#include "math/Conversion.h"

#include "dsp/Context.h"
#include "dsp/oscillator/SquareSincIntegral8.h"
#include "dsp/oscillator/SquareChebyshev8.h"
#include "dsp/cv/LinearSmoother.h"
#include "dsp/cv/PercussionGenerator.h"

#include "control/Midi.h"

namespace Ath::Forma
{
    class FormaSynth : public Processor::MidiAudioProcessor
    {
public:
        // Some parameters can be updated with MIDI CC
        // This class handles CC commands by itself, to make them sample-accurate
        // If some parameter is CCed, synth needs to notify the plugin
        // that this parameter was changed, to propagate the change to the plugin's state
        struct ParameterValueData
        {
            float value = 0;
            bool touched = false;

            void touch(float x)
            {
                auto diff = std::abs(x - value);
                value = x;
                if (diff > 0.001f) touched = true;
            }

            void touchSilently(float x)
            {
                value = x;
            }

            ParameterValueData untouch()
            {
                auto copy = *this;
                touched = false;
                return copy;
            }
        };
private:
        static constexpr int KEY_NUMBER = 61;
        static constexpr int OSC_NUMBER = 12;
        static constexpr int OSC_OUTPUTS_NUMBER = KEY_NUMBER + 12;

        std::array<Dsp::Oscillator::SquareSincIntegral8, OSC_NUMBER> oscillators;
        std::array<Dsp::Oscillator::SquareChebyshev8, OSC_NUMBER> oscillators2;
        std::array<FormaNeedleContacts, KEY_NUMBER> needleContacts;

        std::array<FormaFilterBank, 6> filterBanks;
        FormaTransistorNonlinearity8 filterNonlinearity;
        Dsp::Filter::TPT::LowPass1<float> filterTone;

        Dsp::Cv::PercussionGenerator<double> percussionGenerator;
        FormaTransistorNonlinearity percusionNonlinearity;
        FormaHum hum;
        FormaTransistorNonlinearity outputNonlinearity;
        Dsp::Cv::LinearSmoother<float> gateSmoother;

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
        
        int voiceCount = 0;
        std::array<Simd::float8, KEY_NUMBER> needleContactInputs;
        std::array<Simd::float8, KEY_NUMBER> needleContactOutputs;
        std::array<Simd::float8, KEY_NUMBER> prefilterGains;

        std::array<Simd::float8, 6> filterBankInputs;        
        
        alignas(32) std::array<float, 8> parameterFluteStopsInputs;
        Simd::float8 parameterFluteStops = 0.0f;

        alignas(32) std::array<float, 8> parameterPercStopsInputs;
        Simd::float8 parameterPercStops = 0.0f;

        Simd::float8 keyboardBleedGain = Math::dB(-70);
        Simd::float8 terzBleedGain = Math::dB(-10);
        float noiseFloorGain = Math::dB(-40);

        Simd::float8 parameterDriveGain = 1.0f;

        float gate = 0.0f;

        std::array<ParameterValueData, PARAM_COUNT> parameters;

public:
        FormaSynth();

        void setContext (Dsp::Context context);

        void processBlock (float* buffer, int numberOfSamples) override;

        void handleMidiEvent (Control::Midi::Message message) override;

        void setParameter (int parameterIndex, float x, bool touch = false);

        ParameterValueData getParameter(size_t parameterIndex) { return parameters[parameterIndex].untouch(); }
        
    };
}

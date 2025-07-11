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

        VeronikaSynth()
        {
            for (int i = 0; i < 16; i++)
            {
                voiceManager.noteOn_out(i).addMemberCallback(voices[i], &VeronikaVoice::handleNoteOn);
                voiceManager.noteOff_out(i).addMemberCallback(voices[i], &VeronikaVoice::handleNoteOff);
            }

            parameterFluteStops1Smoother.setCutoffFrequency(100.0f);
            parameterFluteStops2Smoother.setCutoffFrequency(100.0f);
        }

        void setContext(Dsp::Context context)
        {
            for (auto& voice : voices)
            {
                voice.setContext(context);
            }
            phaseCounter.setContext(context);

            timbreProcessor.setContext(context);
            parameterFluteStops1Smoother.setContext(context);
            parameterFluteStops2Smoother.setContext(context);
        }

        void setParameterFlute16(float x)
        {
            float a = 0.1f;
            float b = 1.0f;

            parameterFluteStops1[0] = std::lerp(minVolumeOfStop, 1.0f, x);

        }

        void setParameterFlute8(float x)
        {
            parameterFluteStops1[1] = std::lerp(minVolumeOfStop, 1.0f, x);
        }

        void setParameterFlute4(float x)
        {
            parameterFluteStops1[2] = std::lerp(minVolumeOfStop, 1.0f, x);
        }

        void setParameterFlute2(float x)
        {
            parameterFluteStops1[3] = std::lerp(minVolumeOfStop, 1.0f, x);
        }

        void processBlock(float* buffer, int numberOfSamples) override
        {
            if (!voiceManager.isAtLeastOneVoiceActive()) phaseCounter.reset();

            for (int i = 0; i < numberOfSamples; i++)
            {
                phaseCounter.processSample();

                parameterFluteStops1Smoother.process(parameterFluteStops1);
                parameterFluteStops2Smoother.process(parameterFluteStops2);

                vec4 rawSquare = 0.0f;
                for (auto& voice : voices)
                {
                    if (voice.isActive())
                    {
                        vec4 sample = voice.processSample();
                        timbreProcessor.addSignal(voice.processSample(), voice.getNote());

                        rawSquare += sample;
                    }
                }

                auto sample = (timbreProcessor.processSample()) * parameterFluteStops1Smoother.last();
                buffer[i] = sample.sum();

                //This will synchronize inactive voices with global time
                //So oscillators running at the same frequency will all be in phase
                for (auto& voice : voices)
                {
                    if (!voice.isActive())
                    {
                        voice.setTime(phaseCounter.getTime());
                    }
                }
            }
        }

        void handleMidiEvent(Control::Midi::Message message) override
        {
            if (message.isNoteOnOrOff())
            {
                int noteNumber = message.data1;

                constexpr int lowestNote = Electrophilia::Math::C1_MIDI_NOTE_NUMBER;
                constexpr int highestNote = lowestNote + 61;

                if ((noteNumber >= lowestNote) && (noteNumber <= highestNote))
                {
                     // fire midi events here
                     if (message.isNoteOn())
                     {
                        voiceManager.handleNoteOn(Electrophilia::Control::Midi::MessageNoteOn(message));
                     }
                     if (message.isNoteOff())
                     {
                        voiceManager.handleNoteOff(Electrophilia::Control::Midi::MessageNoteOff(message));
                     }
                }
            }
        }

    };
}

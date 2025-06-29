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

        float parameterFlute16;
        float parameterFlute8;
        float parameterFlute513;
        float parameterFlute4;
        float parameterFlute2;
        float parameterFlute135;

        vec4 parameterFluteStops1;
        vec4 parameterFluteStops2;

        std::array<VeronikaVoice, 16> voices;
        Dsp::PhaseCounter phaseCounter;
        TimbreProcessor timbreProcessor;

        Dsp::Filter1P<vec4> parameterFluteStops1Smoother;
        Dsp::Filter1P<vec4> parameterFluteStops2Smoother;


        static constexpr float minVolumeOfStop = -1000;

public:

        VeronikaSynth()
        {
            for (int i = 0; i < 16; i++)
            {
                voiceManager.noteOn_out(i).addMemberCallback(voices[i], &VeronikaVoice::handleNoteOn);
                voiceManager.noteOff_out(i).addMemberCallback(voices[i], &VeronikaVoice::handleNoteOff);
            }
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
            parameterFluteStops1[0] = Math::linearVolumeToLog(x, minVolumeOfStop);
        }

        void setParameterFlute8(float x)
        {
            parameterFluteStops1[1] = Math::linearVolumeToLog(x, minVolumeOfStop);
        }

        void setParameterFlute4(float x)
        {
            parameterFluteStops1[2] = Math::linearVolumeToLog(x, minVolumeOfStop);
        }

        void setParameterFlute2(float x)
        {
            parameterFluteStops1[3] = Math::linearVolumeToLog(x, minVolumeOfStop);
        }

        void processBlock(float* buffer, int numberOfSamples) override
        {
            if (!voiceManager.isAtLeastOneVoiceActive()) phaseCounter.reset();

            for (int i = 0; i < numberOfSamples; i++)
            {
                phaseCounter.processSample();

                for (auto& voice : voices)
                {
                    if (voice.isActive())
                    {
                        timbreProcessor.addSignal(voice.processSample(), voice.getNote());
                    }
                }

                auto sample = timbreProcessor.processSample();
                buffer[i] = sample;

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

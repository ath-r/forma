#include "VeronikaSynth.h"

namespace Electrophilia::Veronika
{

    VeronikaSynth::VeronikaSynth()
    {
        for (int i = 0; i < 16; i++)
        {
            voiceManager.noteOn_out (i).addMemberCallback (voices[i], &VeronikaVoice::handleNoteOn);
            voiceManager.noteOff_out (i).addMemberCallback (voices[i], &VeronikaVoice::handleNoteOff);
        }

        parameterFluteStops1Smoother.setCutoffFrequency (100.0f);
        parameterFluteStops2Smoother.setCutoffFrequency (100.0f);
    }

    void VeronikaSynth::setContext (Dsp::Context context)
    {
        for (auto& voice : voices)
        {
            voice.setContext (context);
        }
        phaseCounter.setContext (context);

        timbreProcessor1.setContext (context);
        timbreProcessor2.setContext(context);
        parameterFluteStops1Smoother.setContext (context);
        parameterFluteStops2Smoother.setContext (context);

        gateSmoother.setContext(context);
        gateSmoother.setTime(0.001f);
    }

    void VeronikaSynth::setParameterFlute16 (float x) { parameterFluteStops1[0] = std::lerp (minVolumeOfStop, 1.0f, x); }
    void VeronikaSynth::setParameterFlute8 (float x) { parameterFluteStops1[1] = std::lerp (minVolumeOfStop, 1.0f, x); }
    void VeronikaSynth::setParameterFlute4 (float x) { parameterFluteStops1[2] = std::lerp (minVolumeOfStop, 1.0f, x); }
    void VeronikaSynth::setParameterFlute2 (float x) { parameterFluteStops1[3] = std::lerp (minVolumeOfStop, 0.5f, x); }

    void VeronikaSynth::setParameterFlute5 (float x) { parameterFluteStops2[1] = std::lerp (minVolumeOfStop, 1.0f, x);};
    void VeronikaSynth::setParameterFlute1 (float x) { parameterFluteStops2[2] = std::lerp (minVolumeOfStop, 1.0f, x);};

    void VeronikaSynth::processBlock (float* buffer, int numberOfSamples)
    {
        if (voiceManager.isAtLeastOneVoiceActive())
        {
            gate = 1.0f;
            gateSmoother.setTime(0.001f);
        }
        else
        {
            gate = 0.0f;
            gateSmoother.setTime(1.0f);

            phaseCounter.reset();
        }


        for (int i = 0; i < numberOfSamples; i++)
        {
            phaseCounter.processSample();

            parameterFluteStops1Smoother.process (parameterFluteStops1);
            parameterFluteStops2Smoother.process (parameterFluteStops2);

            for (auto& voice : voices)
            {
                if (voice.isActive())
                {
                    vec4 octaves;
                    vec4 mutations;
                    int note = voice.getNote();

                    voice.processSample(octaves, mutations);
                    timbreProcessor1.addSignal (octaves, note);
                    timbreProcessor2.addSignal(mutations, note);
                }
            }

            auto sample = (timbreProcessor1.processSample()) * parameterFluteStops1Smoother.last();
            sample += (timbreProcessor2.processSample()) * parameterFluteStops2Smoother.last();

            buffer[i] = sample.sum() * gateSmoother.process(gate);

            //This will synchronize inactive voices with global time
            //So oscillators running at the same frequency will all be in phase
            for (auto& voice : voices)
            {
                if (!voice.isActive())
                {
                    voice.setTime (phaseCounter.getTime());
                }
            }
        }
    }

    void VeronikaSynth::handleMidiEvent (Control::Midi::Message message)
    {
        if (message.isAllNotesOff())
        {
            voiceManager.handleAllNotesOff(Midi::MessageAllNotesOff (message));
            return;
        }

        if (message.isNoteOnOrOff())
        {
            int noteNumber = message.data1;

            constexpr int lowestNote = Electrophilia::Math::C2_MIDI_NOTE_NUMBER;
            constexpr int highestNote = lowestNote + 61;

            if ((noteNumber >= lowestNote) && (noteNumber <= highestNote))
            {
                // fire midi events here
                if (message.isNoteOn())
                {
                    voiceManager.handleNoteOn (Midi::MessageNoteOn (message));
                }

                if (message.isNoteOff())
                {
                    voiceManager.handleNoteOff (Midi::MessageNoteOff (message));
                }
            }

            return;
        }
    }

}

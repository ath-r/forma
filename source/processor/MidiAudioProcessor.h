#pragma once

#include "../control/Midi.h"

namespace Ath::Processor
{

    class MidiAudioProcessor
    {
    public:
        virtual void processBlock(float * buffer, int numberOfSamples)
        {

        }

        virtual void handleMidiEvent(Control::Midi::Message message)
        {

        }

        virtual void process(float * buffer, int numberOfSamples, Control::Midi::MessageMeta* messages, int numberOfMessages)
        {
            // This loop will iterate over all the midi events AND the audio frames after the last midi event
            // If there are no midi events, then it will just go through the audio frames in one go

            int currentSample = 0;
            for (int i_midi = 0; i_midi < numberOfMessages + 1; i_midi++)
            {
                bool processMidi = i_midi < numberOfMessages;

                int stopAtSample = numberOfSamples;
                if (processMidi)
                {
                    Control::Midi::MessageMeta metadata = messages[i_midi];
                    stopAtSample = metadata.samplePosition;
                }

                //PROCESS AUDIO HERE
                int samplesToProcess = stopAtSample - currentSample;
                processBlock(&buffer[currentSample], samplesToProcess);

                currentSample = stopAtSample;

                if (processMidi)
                {
                    Control::Midi::MessageMeta metadata = messages[i_midi];
                    auto message = metadata.message;

                    //Fire midi event
                    handleMidiEvent(message);
                }

            }

        }

    };
}

#pragma once

#include <deque>

#include "Events.h"
#include "Midi.h"

namespace Electrophilia::Control
{

    class VoiceManager
    {
        struct VoiceController
        {
            bool active = false;
            int pitch = 69; //actually this funny number is the MIDI note number for 440hz
            int channel = 0;

            EventOutput<Midi::MessageNoteOn> noteOn_out;
            EventOutput<Midi::MessageNoteOff> noteOff_out;
        };

        VoiceController voices[16];

        std::deque<VoiceController*> voiceDeque;

    public:

        VoiceManager()
        {
            for (VoiceController& voice : voices)   voiceDeque.push_back(&voice);
        }

        EventOutput<Midi::MessageNoteOn>& noteOn_out(int i) {    return voices[i].noteOn_out;    };
        EventOutput<Midi::MessageNoteOff>& noteOff_out(int i) {    return voices[i].noteOff_out;    };

        void handleNoteOn(const Midi::MessageNoteOn message) 
        {
            auto& voice = voiceDeque.front();

            if (!voice->active)
            {
                voice->active = true;
                voice->pitch = message.note;
                voice->noteOn_out.fire(message);

                voiceDeque.pop_front();
                voiceDeque.push_back(voice);
            }
        };

        void handleNoteOff(const Midi::MessageNoteOff message) 
        {
            for (VoiceController& voice : voices)
            {
                if (voice.pitch == message.note)
                {
                    voice.active = false;
                    voice.noteOff_out.fire(message);
                    //break;
                }
            }
        };

    };
}
#pragma once

#include <array>
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
            int note = 69;
            int channel = 0;

            EventOutput<Midi::MessageNoteOn> noteOn_out;
            EventOutput<Midi::MessageNoteOff> noteOff_out;
        };

        std::array<VoiceController, 16> voices;

        std::deque<VoiceController*> voiceDeque;

    public:

        VoiceManager()
        {
            for (VoiceController& voice : voices)   voiceDeque.push_back(&voice);
        }

        EventOutput<Midi::MessageNoteOn>& noteOn_out(int i) {    return voices[i].noteOn_out;    };
        EventOutput<Midi::MessageNoteOff>& noteOff_out(int i) {    return voices[i].noteOff_out;    };

        bool isAtLeastOneVoiceActive()
        {
            for (VoiceController& voice : voices)
            {
                if (voice.active) return true;
            }
            return false;
        }

        void handleNoteOn(const Midi::MessageNoteOn message)
        {
            auto& voice = voiceDeque.front();

            if (!voice->active)
            {
                voice->active = true;
                voice->note = message.note;
                voice->noteOn_out.fire(message);

                voiceDeque.pop_front();
                voiceDeque.push_back(voice);
            }
        };

        void handleNoteOff(const Midi::MessageNoteOff message)
        {
            for (VoiceController& voice : voices)
            {
                if (voice.note == message.note)
                {
                    voice.active = false;
                    voice.noteOff_out.fire(message);
                    //break;
                }
            }
        };

    };
}

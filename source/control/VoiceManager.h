#pragma once

#include <array>

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

    public:
        EventOutput<Midi::MessageNoteOn>& noteOn_out (int i);
        EventOutput<Midi::MessageNoteOff>& noteOff_out (int i);

        bool isAtLeastOneVoiceActive();

        void handleNoteOn (const Midi::MessageNoteOn message);

        void handleNoteOff (const Midi::MessageNoteOff message);
    };
}

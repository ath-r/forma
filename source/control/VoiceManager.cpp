#include "VoiceManager.h"

namespace Electrophilia::Control
{

    EventOutput<Midi::MessageNoteOn>& VoiceManager::noteOn_out (int i) { return voices[i].noteOn_out; };
    EventOutput<Midi::MessageNoteOff>& VoiceManager::noteOff_out (int i) { return voices[i].noteOff_out; };

    bool VoiceManager::isAtLeastOneVoiceActive()
    {
        for (VoiceController& voice : voices)
        {
            if (voice.active)
                return true;
        }
        return false;
    }

    void VoiceManager::handleNoteOn (const Midi::MessageNoteOn message)
    {
        for (auto& voice : voices)
        {
            if (!voice.active)
            {
                voice.active = true;
                voice.note = message.note;
                voice.noteOn_out.fire (message);

                break;
            }
        }
    };

    void VoiceManager::handleNoteOff (const Midi::MessageNoteOff message)
    {
        for (auto& voice : voices)
        {
            if (voice.note == message.note && voice.active)
            {
                voice.active = false;
                voice.noteOff_out.fire (message);

                break;
            }
        }
    };
}

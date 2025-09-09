#include "Midi.h"

namespace Ath::Control::Midi
{

    Message Message::fromRawData (const unsigned char* data, int size)
    {
        Message message;

        if (size > 0)
        {
            message.status = data[0];
        }

        if (size > 1)
        {
            message.data1 = data[1];
        }

        if (size > 2)
        {
            message.data2 = data[2];
        }

        return message;
    }

    MessageType Message::type() const { return static_cast<MessageType> (status & 0xF0); }

    unsigned char Message::channel() const { return status & 0x0F; }

    bool Message::isNoteOn() const { return type() == MessageType::NoteOn; }

    bool Message::isNoteOff() const { return type() == MessageType::NoteOff; }

    bool Message::isNoteOnOrOff() const { return isNoteOn() || isNoteOff(); }

    bool Message::isControlChange() const { return type() == MessageType::ControlChange; }

    bool Message::isAllNotesOff() const
    {
        return isControlChange() && (data1 == static_cast<unsigned char> (ChannelModeMessages::AllNotesOff));
    }

}

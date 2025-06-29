#include "Midi.h"

namespace Electrophilia::Control::Midi
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

    MessageType Message::type() { return static_cast<MessageType> (status & 0xF0); }

    unsigned char Message::channel() { return status & 0x0F; }

    bool Message::isNoteOn() { return type() == MessageType::NoteOn; }

    bool Message::isNoteOff() { return type() == MessageType::NoteOff; }

    bool Message::isNoteOnOrOff() { return isNoteOn() || isNoteOff(); }
}

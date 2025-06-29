#pragma once

namespace Electrophilia::Control::Midi
{
    enum class MessageType : unsigned char
    {
        NoteOff = 0x80,
        NoteOn = 0x90,
        Aftertouch = 0xA0,
        ControlChange = 0xB0,
        ProgramChange = 0xC0,
        ChannelAftertouch = 0xD0,
        PitchBend = 0xE0
    };

    struct MessageNoteOn
    {
        unsigned char channel;
        unsigned char note;
        unsigned char velocity;
    };

    struct MessageNoteOff
    {
        unsigned char channel;
        unsigned char note;
        unsigned char velocity;
    };

    struct Message
    {
        unsigned char status = 0;
        unsigned char data1 = 0;
        unsigned char data2 = 0;

        static Message fromRawData (const unsigned char* data, int size);

        MessageType type();

        unsigned char channel();

        bool isNoteOn();

        bool isNoteOff();

        bool isNoteOnOrOff();

        explicit operator MessageNoteOn() const
        {
            return MessageNoteOn
            {
                .channel = static_cast<unsigned char>(status & 0x0F),
                .note = data1,
                .velocity = data2
            };
        }

        explicit operator MessageNoteOff() const
        {
            return MessageNoteOff
            {
                .channel = static_cast<unsigned char>(status & 0x0F),
                .note = data1,
                .velocity = data2
            };
        }
    };

    struct MessageMeta
    {
        Message message;
        int samplePosition;
    };
}

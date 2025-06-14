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
        unsigned char status;
        unsigned char data1;
        unsigned char data2;

        MessageType type()
        {
            return static_cast<MessageType>(status & 0xF0);
        }

        unsigned char channel()
        {
            return status & 0x0F;
        }

        bool isNoteOn()
        {
            return type() == MessageType::NoteOn;
        }

        explicit operator MessageNoteOn() const
        {
            return MessageNoteOn
            {
                .channel = status & 0x0F,
                .note = data1,
                .velocity = data2
            };
        }

        bool isNoteOff()
        {
            return type() == MessageType::NoteOff;
        }

        explicit operator MessageNoteOff() const
        {
            return MessageNoteOff
            {
                .channel = status & 0x0F,
                .note = data1,
                .velocity = data2
            };
        }
    };
}


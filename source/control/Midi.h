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

        static Message fromRawData(const unsigned char* data, int size)
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

        bool isNoteOff()
        {
            return type() == MessageType::NoteOff;
        }

        bool isNoteOnOrOff()
        {
            return isNoteOn() || isNoteOff();
        }

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
}

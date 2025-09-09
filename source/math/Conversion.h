#pragma once

#include <cmath>
namespace Electrophilia::Math
{
    static constexpr int C1_MIDI_NOTE_NUMBER = 24;
    static constexpr int C2_MIDI_NOTE_NUMBER = C1_MIDI_NOTE_NUMBER + 12;
    static constexpr int A4_MIDI_NOTE_NUMBER = 69;

    float noteToFrequency (int p, float referenceFrequency = 440.0f);

    float frequencyToNote (float freq, float referenceFrequency = 440.0f);

    float semitonesToFrequencyRatio (float semitones);

    static constexpr float DB_PLUS3 = 1.412537545f;

    static constexpr float DB_MINUS3 = 0.707945784f;
    static constexpr float DB_MINUS6 = 0.501187234f;
    static constexpr float DB_MINUS9 = DB_MINUS3 * DB_MINUS6;
    static constexpr float DB_MINUS12 = DB_MINUS6 * DB_MINUS6;
    static constexpr float DB_MINUS18 = DB_MINUS12 * DB_MINUS6;
    static constexpr float DB_MINUS24 = DB_MINUS12 * DB_MINUS12;
    static constexpr float DB_MINUS30 = DB_MINUS24 * DB_MINUS6;
    static constexpr float DB_MINUS48 = DB_MINUS30 * DB_MINUS18;


    static constexpr float DB_MINUS60 = DB_MINUS30 * DB_MINUS30;
    static constexpr float DB_MINUS72 = DB_MINUS60 * DB_MINUS12;
    static constexpr float DB_MINUS84 = DB_MINUS60 * DB_MINUS30;

    static constexpr float DB_MINUS50 = 0.003162278f;



    float amplitudeToDecibels (float gain);

    float decibelsToAmplitude (float db);

    float amplitudeRatioToDecibels (float ratio);

    float amplitudeRatioToDecibels (float v1, float v2);

    float decibelsToAmplitudeRatio (float db);

    float linearVolumeToLog (float x, float dB_at0);

    float logerp2 (float a, float b, float x);

    float logerp10 (float a, float b, float x);

}

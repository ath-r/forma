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

    static constexpr float DB_MINUS6 = 0.501187234f;
    static constexpr float DB_MINUS12 = 0.251188643f;
    static constexpr float DB_MINUS18 = 0.125892541f;
    static constexpr float DB_MINUS24 = 0.063095734f;
    static constexpr float DB_MINUS30 = 0.031622777f;
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

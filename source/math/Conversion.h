#pragma once

#include <cmath>
namespace Electrophilia::Math
{
    static constexpr int C1_MIDI_NOTE_NUMBER = 24;
    static constexpr int C2_MIDI_NOTE_NUMBER = C1_MIDI_NOTE_NUMBER + 12;
    static constexpr int A4_MIDI_NOTE_NUMBER = 69;

    float noteToFrequency (const int p, float referenceFrequency = 440.0f);

    float frequencyToNote (float freq, float referenceFrequency = 440.0f);

    float semitonesToFrequencyRatio (float semitones);

    float amplitudeToDecibels (float gain);

    float decibelsToAmplitude (float db);

    float amplitudeRatioToDecibels (float ratio);

    float amplitudeRatioToDecibels (float v1, float v2);

    float decibelsToAmplitudeRatio (float db);

    float linearVolumeToLog (float x, float dB_at0);
}

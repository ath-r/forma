#include "Conversion.h"

#include <cmath>

namespace Ath::Math
{
    float noteToFrequency (const int p, float referenceFrequency)
    {
        return referenceFrequency * std::pow (2.0f, static_cast<float> (p - A4_MIDI_NOTE_NUMBER) / 12.0);
    }

    float frequencyToNote (float freq, float referenceFrequency)
    {
        return A4_MIDI_NOTE_NUMBER + 12.0f * std::log2 (freq / referenceFrequency);
    }

    float semitonesToFrequencyRatio (float semitones) { return std::pow (2.0f, semitones / 12.0f); }

    float amplitudeToDecibels (float gain) { return 20.0f * std::log10 (gain); }

    float decibelsToAmplitude (float db) { return std::pow (10.0f, db / 20.0f); }

    float amplitudeRatioToDecibels (float ratio) { return 10.0f * std::log10 (ratio); }

    float amplitudeRatioToDecibels (float v1, float v2) { return 10.0f * std::log10 (v1 / v2); }

    float decibelsToAmplitudeRatio (float db) { return std::pow (10.0f, db / 10.0f); }

    float linearVolumeToLog (float x, float dB_at0)
    {
        const float dB = std::lerp (dB_at0, 0.0f, x);
        return decibelsToAmplitude (dB);
    }

    float logerp2 (float a, float b, float x)
    {
        const float log = std::log2 (b / a);

        return a * std::pow (10, log * x);
    }

    float logerp10 (float a, float b, float x)
    {
        const float log = std::log10 (b / a);

        return a * std::pow (10, log * x);
    }
}

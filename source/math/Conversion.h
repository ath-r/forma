#pragma once

#include <cmath>
namespace Ath::Math
{
    static constexpr int C1_MIDI_NOTE_NUMBER = 24;
    static constexpr int C2_MIDI_NOTE_NUMBER = C1_MIDI_NOTE_NUMBER + 12;
    static constexpr int A4_MIDI_NOTE_NUMBER = 69;

    float noteToFrequency (int p, float referenceFrequency = 440.0f);

    float noteToFrequency (float p, float referenceFrequency = 440.0f);

    float frequencyToNote (float freq, float referenceFrequency = 440.0f);

    float semitonesToFrequencyRatio (float semitones);

    constexpr double ipow(double a, int n) 
    {
        if (n == 0) return 1.0;
        if (n < 0) return 1.0 / ipow(a, -n);
        if (n % 2 == 0) 
        {
            double half = ipow(a, n / 2);
            return half * half;
        }
        return a * ipow(a, n - 1);
    }   

    constexpr float dB(int dB)
    {
        constexpr float base = 1.1220184543019633f; // 10^(1/20)
        return ipow(base, dB);
    }

    static constexpr float DB_MINUS1 = 0.891250938f;
    static constexpr float DB_MINUS2 = 0.794328235f;
    static constexpr float DB_MINUS3 = 0.707945784f;
    static constexpr float DB_MINUS6 = 0.501187234f;
    static constexpr float DB_MINUS9 = DB_MINUS3 * DB_MINUS6;
    static constexpr float DB_MINUS12 = DB_MINUS6 * DB_MINUS6;
    static constexpr float DB_MINUS18 = DB_MINUS12 * DB_MINUS6;
    static constexpr float DB_MINUS24 = DB_MINUS18 * DB_MINUS6;
    static constexpr float DB_MINUS30 = DB_MINUS24 * DB_MINUS6;
    static constexpr float DB_MINUS36 = DB_MINUS30 * DB_MINUS6;
    static constexpr float DB_MINUS42 = DB_MINUS36 * DB_MINUS6;
    static constexpr float DB_MINUS48 = DB_MINUS42 * DB_MINUS6;
    static constexpr float DB_MINUS54 = DB_MINUS48 * DB_MINUS6;
    static constexpr float DB_MINUS60 = DB_MINUS54 * DB_MINUS6;
    static constexpr float DB_MINUS66 = DB_MINUS60 * DB_MINUS6;
    static constexpr float DB_MINUS72 = DB_MINUS66 * DB_MINUS6;
    static constexpr float DB_MINUS78 = DB_MINUS72 * DB_MINUS6;
    static constexpr float DB_MINUS84 = DB_MINUS78 * DB_MINUS6;

    static constexpr float DB_MINUS50 = 0.003162278f;

    static constexpr float DB_PLUS1 = 1.0f / DB_MINUS1;
    static constexpr float DB_PLUS2 = 1.0f / DB_MINUS2;
    static constexpr float DB_PLUS3 = 1.0f / DB_MINUS3;

    float amplitudeToDecibels (float gain);

    float decibelsToAmplitude (float db);

    float amplitudeRatioToDecibels (float ratio);

    float amplitudeRatioToDecibels (float v1, float v2);

    float decibelsToAmplitudeRatio (float db);

    float linearVolumeToLog (float x, float dB_at0);

    float logerp2 (float a, float b, float x);

    float logerp10 (float a, float b, float x);

}

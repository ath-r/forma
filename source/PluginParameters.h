#pragma once
#include <cmath>

#include "control/Parameter.h"

namespace Ath::Forma
{

    static const Control::Parameter Flute16
    {
        .name = "16′",
        .id = "f16",
        .def = 1.0f
    };

    static const Control::Parameter Flute8
    {
        .name = "8′",
        .id = "f8",
        .def = 1.0f
    };

    static const Control::Parameter Flute5
    {
        .name = "5⅓′",
        .id = "f5",
        .def = 1.0f
    };

    static const Control::Parameter Flute4
    {
        .name = "4′",
        .id = "f4",
        .def = 1.0f
    };

    static const Control::Parameter Flute2
    {
        .name = "2′",
        .id = "f2",
        .def = 1.0f
    };

    static const Control::Parameter Flute1
    {
        .name = "1⅗′",
        .id = "f1",
        .def = 1.0f
    };

    static const Control::Parameter Tone
    {
        .name = "TONE",
        .id = "tone",
        .def = 0.0f
    };

    static const Control::Parameter PercussionOn
    {
        .type = Control::Parameter::Type::Bool,
        .name = "Percussion On/Off",
        .id = "percEnabled",
        .offText = "OFF",
        .onText = "ON"
    };

    static const Control::Parameter PercussionCV
    {
        .type = Control::Parameter::Type::Float,
        .category = Control::Parameter::Category::OtherMeter,
        .name = "Percussion CV",
        .id = "percCv"
    };

    static const Control::Parameter Percussion16
    {
        .name = "P16′",
        .id = "p16"
    };

    static const Control::Parameter Percussion8
    {
        .name = "P8′",
        .id = "p8"
    };

    static const Control::Parameter Percussion5
    {
        .name = "P5⅓′",
        .id = "p5"
    };

    static const Control::Parameter Percussion4
    {
        .name = "P4′",
        .id = "p4"
    };

    static const Control::Parameter Percussion2
    {
        .name = "P2′",
        .id = "p2"
    };

    static const Control::Parameter Percussion1
    {
        .name = "P1⅗′",
        .id = "p1"
    };

    static const Control::Parameter PercussionTime
    {
        .name = "TIME",
        .id = "time",
        .def = 0.1f
    };

    static const Control::Parameter PercussionCresc
    {
        .type = Control::Parameter::Type::Bool,
        .name = "Decay/Crescendo",
        .id = "percCresc",
        .offText = "DECAY",
        .onText = "CRESC",
    };

    static const Control::Parameter PercussionSoft
    {
        .type = Control::Parameter::Type::Bool,
        .name = "Percussion Soft",
        .id = "percSoft",
        .offText = "SOFT",
        .onText = "BRIGHT"
    };

    static const Control::Parameter PercussionSpeed
    {
        .type = Control::Parameter::Type::Bool,
        .name = "Percussion Speed",
        .id = "percSpeed",
        .offText = "SLOW",
        .onText = "FAST"
    };

    static const Control::Parameter PercussionHarmonic
    {
        .type = Control::Parameter::Type::Bool,
        .name = "Percussion Harmonic",
        .id = "percHarmonic",
        .offText = "2ND",
        .onText = "3RD"
    };

    static const Control::Parameter keyboardBleed
    {
        .name = "Keyswitch Bleed",
        .id = "keyswitchBleed",
        .unit = "dB",
        .def = -70.0f,
        .min = -120.0f,
        .max = -30.0f
    };

    static const Control::Parameter terzBleed
    {
        .name = "Terz Bleed",
        .id = "terzBleed",
        .unit = "dB",
        .def = -30.0f,
        .min = -120.0f,
        .max = 0.0f
    };

    static const Control::Parameter noiseFloor
    {
        .name = "Noise Floor",
        .id = "noiseFloor",
        .unit = "dB",
        .def = -40.0f,
        .min = -120.0f,
        .max = -30.0f
    };

    enum ParameterIDs
    {
        F16,
        F8,
        F5,
        F4,
        F2,
        F1,
        TONE,
        PERC_ON,
        PERC_CV,
        P16,
        P8,
        P5,
        P4,
        P2,
        P1,
        PERC_TIME,
        PERC_CRESC,
        PERC_SOFT,
        PERC_SPEED,
        PERC_HARMONIC,
        BLEED_KEYBOARD,
        BLEED_TERZ,
        NOISE_FLOOR,
        PARAM_COUNT
    };

    static const Control::Parameter ParametersByID[PARAM_COUNT]
    {
        [F16] = Flute16,
        [F8] = Flute8,
        [F5] = Flute5,
        [F4] = Flute4,
        [F2] = Flute2,
        [F1] = Flute1,
        [TONE] = Tone,
        [PERC_ON] = PercussionOn,
        [PERC_CV] = PercussionCV,
        [P16] = Percussion16,
        [P8] = Percussion8,
        [P5] = Percussion5,
        [P4] = Percussion4,
        [P2] = Percussion2,
        [P1] = Percussion1,
        [PERC_TIME] = PercussionTime,
        [PERC_CRESC] = PercussionCresc,
        [PERC_SOFT] = PercussionSoft,
        [PERC_SPEED] = PercussionSpeed,
        [PERC_HARMONIC] = PercussionHarmonic,
        [BLEED_KEYBOARD] = keyboardBleed,
        [BLEED_TERZ] = terzBleed,
        [NOISE_FLOOR] = noiseFloor
    };
}

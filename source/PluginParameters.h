#pragma once
#include <cmath>

#include "control/Parameter.h"

namespace Ath::Forma
{

    static const Control::Parameter Flute16
    {
        .name = "16′",
        .id = "f16",
        .def = 1.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter Flute8
    {
        .name = "8′",
        .id = "f8",
        .def = 1.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter Flute5
    {
        .name = "5⅓′",
        .id = "f5",
        .def = 1.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter Flute4
    {
        .name = "4′",
        .id = "f4",
        .def = 1.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter Flute2
    {
        .name = "2′",
        .id = "f2",
        .def = 1.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter Flute1
    {
        .name = "1⅗′",
        .id = "f1",
        .def = 1.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter Tone
    {
        .name = "TONE",
        .id = "tone",
        .def = 0.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter Percussion16
    {
        .name = "P16′",
        .id = "p16",
        .def = 0.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter Percussion8
    {
        .name = "P8′",
        .id = "p8",
        .def = 0.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter Percussion5
    {
        .name = "P5⅓′",
        .id = "p5",
        .def = 0.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter Percussion4
    {
        .name = "P4′",
        .id = "p4",
        .def = 0.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter Percussion2
    {
        .name = "P2′",
        .id = "p2",
        .def = 0.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter Percussion1
    {
        .name = "P1⅗′",
        .id = "p1",
        .def = 0.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter PercussionTime
    {
        .name = "TIME",
        .id = "time",
        .def = 0.1f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter PercussionPoly
    {
        .type = Control::Parameter::Type::Bool,
        .name = "POLY",
        .id = "poly",
        .offText = "MONO",
        .onText = "POLY",
        .def = 0.0f
    };

    static const Control::Parameter PercussionCresc
    {
        .type = Control::Parameter::Type::Bool,
        .name = "CRESC",
        .id = "cresc",
        .offText = "PERC",
        .onText = "CRESC",
        .def = 0.0f
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
        .def = -10.0f,
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
        P16,
        P8,
        P5,
        P4,
        P2,
        P1,
        TIME,
        POLY,
        CRESC,
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
        [P16] = Percussion16,
        [P8] = Percussion8,
        [P5] = Percussion5,
        [P4] = Percussion4,
        [P2] = Percussion2,
        [P1] = Percussion1,
        [TIME] = PercussionTime,
        [POLY] = PercussionPoly,
        [CRESC] = PercussionCresc,
        [BLEED_KEYBOARD] = keyboardBleed,
        [BLEED_TERZ] = terzBleed,
        [NOISE_FLOOR] = noiseFloor
    };


}

#pragma once
#include <cmath>

#include "control/Parameter.h"

namespace Ath::Forma
{

    static const Control::Parameter TestParameter
    {
        .name = "Frequency",
        .id = "test",
        .unit = "hz",
        .def = 16.0f,
        .min = 0.0625f,
        .max = 16.0f
    };

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

    static const Control::Parameter Drive
    {
        .name = "DRIVE",
        .id = "drive",
        .def = 0.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    static const Control::Parameter Tone
    {
        .name = "TONE",
        .id = "tone",
        .def = 1.0f,
        .min = 0.0f,
        .max = 1.0f
    };

    enum ParameterIDs
    {
        TEST_ID,
        F16,
        F8,
        F5,
        F4,
        F2,
        F1,
        TONE,
        DRIVE,
        PARAM_COUNT
    };

    static const Control::Parameter ParametersByID[PARAM_COUNT]
    {
        [TEST_ID] = TestParameter,
        [F16] = Flute16,
        [F8] = Flute8,
        [F5] = Flute5,
        [F4] = Flute4,
        [F2] = Flute2,
        [F1] = Flute1,
        [TONE] = Tone,
        [DRIVE] = Drive
    };


}

#pragma once
#include <cmath>
#include <string>
#include <format>

struct ParameterData
{
    int hints;
    std::string name;
    std::string id;
    std::string unit;
    float def;
    float min;
    float max;

    std::string getStringFromValue(float value) const
    {
        return std::format("{:.2f}", value);
    }

};

namespace Electrophilia::Plugin::Tremolo
{
    static const ParameterData TestParameter
    {
        .name = "Frequency",
        .id = "test",
        .unit = "hz",
        .def = 16.0f,
        .min = 0.0625f,
        .max = 16.0f
    };

    static const ParameterData Flute16
    {
        .name = "16′",
        .id = "f16",
        .def = 10.0f,
        .min = 0.0f,
        .max = 10.0f
    };

    static const ParameterData Flute8
    {
        .name = "8′",
        .id = "f8",
        .def = 10.0f,
        .min = 0.0f,
        .max = 10.0f
    };

    static const ParameterData Flute5
    {
        .name = "5⅓′",
        .id = "f5",
        .def = 10.0f,
        .min = 0.0f,
        .max = 10.0f
    };

    static const ParameterData Flute4
    {
        .name = "4′",
        .id = "f4",
        .def = 10.0f,
        .min = 0.0f,
        .max = 10.0f
    };

    static const ParameterData Flute2
    {
        .name = "2′",
        .id = "f2",
        .def = 10.0f,
        .min = 0.0f,
        .max = 10.0f
    };

    static const ParameterData Flute1
    {
        .name = "1⅗′",
        .id = "f1",
        .def = 10.0f,
        .min = 0.0f,
        .max = 10.0f
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
        PARAM_COUNT
    };

    static const ParameterData ParametersByID[PARAM_COUNT]
    {
        [TEST_ID] = TestParameter,
        [F16] = Flute16,
        [F8] = Flute8,
        [F5] = Flute5,
        [F4] = Flute4,
        [F2] = Flute2,
        [F1] = Flute1
    };


}

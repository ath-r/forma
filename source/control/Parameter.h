#pragma once

#include <string>
#include <format>

namespace Electrophilia::Control
{
    struct Parameter
    {
        int hints;
        std::string name;
        std::string id;
        std::string unit;
        float def;
        float min;
        float max;

        static std::string getStringFromValue (float value);
    };
}

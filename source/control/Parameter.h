#pragma once

#include <string>
#include <vector>
#include <format>

namespace Ath::Control
{
    struct Parameter
    {
        enum class Type 
        {
            Float,
            Bool,
            Choice
        };

        int hints;

        Type type = Type::Float;
        std::string name;
        std::string id;
        std::string unit;

        std::string offText;
        std::string onText;

        std::vector<std::string> choices;

        float def;
        float min;
        float max;

        static std::string getStringFromValue (float value);
    };
}

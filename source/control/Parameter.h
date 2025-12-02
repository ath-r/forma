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

        enum class Category
        {
            Generic,
            InputGain,
            OutputGain,
            InputMeter,
            OutputMeter,
            CompressorLimiterGainReductionMeter,
            ExpanderGateGainReductionMeter,
            AnalysisMeter, 	
            OtherMeter
        };

        int hints;

        Type type = Type::Float;
        Category category = Category::Generic;
        std::string name;
        std::string id;
        std::string unit;

        std::string offText;
        std::string onText;

        std::vector<std::string> choices;

        float def = 0.0f;
        float min = 0.0f;
        float max = 1.0f;

        static std::string getStringFromValue (float value);
    };
}

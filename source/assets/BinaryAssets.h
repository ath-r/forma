#pragma once

namespace BinaryAssets
{
    static const u_char RussoOneCustomData[] = 
    { 
        #include "RussoOne-Custom.data" 
    };

    static const size_t RussoOneCustomSize = sizeof(RussoOneCustomData) / sizeof(char);

    static const u_char OxaniumBoldData[] = 
    { 
        #include "Oxanium-Bold.data" 
    };

    static const size_t OxaniumBoldSize = sizeof(OxaniumBoldData) / sizeof(char);
}
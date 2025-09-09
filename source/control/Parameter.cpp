#include "Parameter.h"

namespace Ath::Control
{

    std::string Parameter::getStringFromValue (float value) { return std::format ("{:.2f}", value); }
}

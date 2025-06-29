#include "Parameter.h"

namespace Electrophilia::Control
{

    std::string Parameter::getStringFromValue (float value) { return std::format ("{:.2f}", value); }
}

#include "Context.h"
#include "SIMD.h"

#include "Filter.h"
#include <array>

namespace Electrophilia::Dsp::Oscillator
{
    class SquareSimd
    {

        Context c = Context(48000.0f);

        vec4 n;
        vec4 r;

        vec4 phase = 0.0;
        vec4 frequency = 0.0f;
        vec4 delta = 0.0f;

        vec4 out = 0.0f;
        vec4 lastOut = 0.0f;

        static constexpr int blepLength = 4;
        std::array<vec4, blepLength> blepBuffer;

        float _time;

    public:
        void setContext (const Context context);

        void setFrequency (const vec4 newFrequency);

        void setTime (const double time);

        vec4 processSample();

    private:
        vec4 foldArgument (vec4 x);

        vec4 sinc (vec4 x);

        vec4 sin2pi9 (vec4 x);

        vec4 dirichlet (vec4 x);

        vec4 sincIntegral (vec4 x);
    };
}

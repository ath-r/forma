#include "Context.h"
#include "SIMD.h"

#include "Filter.h"

namespace Electrophilia::Dsp::Oscillator
{
    class BlitSquareSimd
    {

        Context c = Context(48000.0f);

        Filter1P<vec4> filter1, filter2;

        vec4 n;

        vec4 phase = 0.0;
        vec4 frequency = 0.0f;
        vec4 delta = 0.0f;

        vec4 out = 0.0f;
        vec4 lastOut = 0.0f;

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
    };
}

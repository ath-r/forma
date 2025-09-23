#include "../Context.h"

namespace Ath::Dsp::Oscillator
{
    class SquareSincIntegral
    {

        Context c = Context(48000.0f);

        float n;

        float phase = 0.0;
        float frequency = 0.0f;
        float delta = 0.0f;

    public:
        void setContext (const Context context);

        void setFrequency (const float newFrequency);

        void setTime (const double time);

        float processSample();

    private:

        float sincIntegral (float x);
    };
}

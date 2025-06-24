#pragma once

#include "../dsp/Context.h"
#include "../math/Math.h"

#include "../dsp/cv/LinearSmoother.h"

namespace Electrophilia::Dsp
{
    class BrokenGate
    {
        Context c = Context(48000.0f);

        bool active = false;

        bool on = false;

        float maxDelay = 0.0f;
        float delay = 0.0f;

        Cv::LinearSmoother<float> smoother;

        public:

        BrokenGate()
        {
            const auto rand = std::abs(Math::fastRandomFloat());

            float delay = rand * rand;
        }

        void setContext(Context newContext)
        {
            c = newContext;

            setMaxDelay(maxDelay);
        }

        void setMaxDelay(float newMaxDelay)
        {
            maxDelay = newMaxDelay;

            smoother.setTime(maxDelay);
        }

        void noteOn()
        {
            on = true;
            active = true;
            smoother.setTargetValue(1.0f);
        }

        void noteOff()
        {
            on = false;
            smoother.setTargetValue(0.0f);
        }

        float isActive()
        {
            return active;
        }

        float processSample()
        {
            auto slew = smoother.process();
            auto slewAboveDelay = (slew >= delay);

            active = (on || slewAboveDelay);

            return (on && slewAboveDelay) ? 1.0f : 0.0f;
        }

        float processSample(float x)
        {
            return x * processSample();
        }

        void processBlock(float* writePointer, int startSample, int numSamples)
        {
            for (int i = startSample; i < (startSample + numSamples); ++i)
            {
                writePointer[i] = processSample(writePointer[i]);
            }
        }
    };
}

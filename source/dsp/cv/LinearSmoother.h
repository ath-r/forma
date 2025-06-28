#pragma once

#include "../Context.h"
#include <algorithm>

namespace Electrophilia::Dsp::Cv
{
    template <typename T>
    class LinearSmoother
    {
        Context c = Context(48000.0f);
        T targetValue = 0.0;
        T currentValue = 0.0;

        T time = 0.0;
        T delta = 0.0;

    public:

        void reset()
        {
            currentValue = 0.0f;
            targetValue = 0.0f;
        }

        void setContext(const Context context)
        {
            c = context;
            setTime(time);
        }

        virtual void setTime(T newTime)
        {
            time = newTime;

            delta = c.T / time;
        }

        virtual inline void setTargetValue(T value)
        {
            targetValue = value;
        }

        inline T last()
        {
            return currentValue;
        }

        virtual inline T process()
        {
            auto diff = targetValue - currentValue;
            diff = std::clamp(diff, -delta, delta);
            currentValue += diff;

            return currentValue;
        };

        virtual inline T process(const T value)
        {
            setTargetValue(value);
            return process();
        }
    };
}

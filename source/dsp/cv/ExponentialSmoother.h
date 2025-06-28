#pragma once

#include "../Filter.h"

namespace Electrophilia::Dsp::Cv
{
    template <typename T>
    class ExponentialSmoother : private Filter1P<T>
    {
        float targetValue = 0.0f;
        float time = 0.01f;

    public:

        void reset()
        {
            Filter1P<T>::reset();
        }

        void setContext(const Context context)
        {
            Filter1P<T>::setContext(context);
            Filter1P<T>::setTime(time);
        }

        virtual void setTime(T newTime)
        {
            time = newTime;
            Filter1P<T>::setTime(time);
        }

        virtual inline void setTargetValue(T value)
        {
            targetValue = value;
        }

        inline T last()
        {
            return Filter1P<T>::last();
        }

        virtual inline T process()
        {
            return Filter1P<T>::process(targetValue);
        };

        virtual inline T process(const T value)
        {
            setTargetValue(value);
            return process();
        }
    };
}

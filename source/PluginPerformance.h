#pragma once

#include <atomic>

namespace Ath::Forma
{  
    struct Measurement
    {
        void reset() noexcept
        {
            atom = 0.0f;
        }

        void write(float newValue) noexcept
        {
            value = newValue;
            atom.store(newValue);
        }

        float read() noexcept
        {
            return atom.load();
        }

        std::atomic<float> atom = 0.0f;

        float value = 0.0f;
        float oldValue = 0.0f;
    };

    struct PerformanceData
    {
        Measurement SampleRate;
        Measurement SamplesPerBlock;
        Measurement ExecutionTimeImmediate;
        Measurement ExecutionTime100;
        Measurement ExecutionTime1000;
        Measurement MaxExecutionTimeNs;
        Measurement MinExecutionTimeNs;
    };

}
#include "FormaSynth.h"
#include "control/Midi.h"
#include "math/Conversion.h"
#include "math/Random.h"
#include "math/Simd.h"

namespace Ath::Forma
{

    FormaSynth::FormaSynth()
    {
        
    }

    void FormaSynth::setContext (Dsp::Context context)
    {
        phaseCounter.setContext (context);

        for (int i = 0; i < OSC_NUMBER; i++)
        {
            auto& osc = oscillators[i];

            osc.setContext(context);

            Simd::float8 baseFrequency = Math::noteToFrequency(Math::C1_MIDI_NOTE_NUMBER + i * 8);
            float sqrt2 = 1.414213562f;
            Simd::float8 multipliers = {
                1.0f, 
                1.059463094f, 
                1.122462048f, 
                1.189207115f, 
                1.25992105f, 
                1.334839854f, 
                1.414213562f, 
                1.498307077f
            };
            Simd::float8 frequencies = baseFrequency * multipliers;
            
            osc.setFrequency(frequencies);
        }
        
        for (auto& smoother : parameterSmootherFluteStops)
        {
            smoother.setContext(context);
            smoother.setTime(0.002f);
        }

        gateSmoother.setContext(context);
        gateSmoother.setTime(0.001f);
    }

    void FormaSynth::processBlock (float* buffer, int numberOfSamples)
    {
        if (true)
        {
            gate = 1.0f;
            gateSmoother.setTime(0.001f);
        }
        else
        {
            gate = 0.0f;
            gateSmoother.setTime(1.0f);

            //reset global time when all voices are inactive:
            phaseCounter.reset();
        }

        //sync oscillators to global time:
        //for (auto& osc : oscillators) osc.setTime(phaseCounter.getTime());

        for (int i = 0; i < numberOfSamples; i++)
        {
            //process phase counter:
            phaseCounter.processSample(); 

            //process parameter smoothers:
            for (auto& smoother : parameterSmootherFluteStops)
            {
                smoother.process();
            }

            buffer[i] = 0.0f;

            //process oscillators:
            for (int n = 0; n < OSC_NUMBER; n++)
            {
                auto sample = oscillators[n].processSample();
                oscillatorOutputs[n] = sample;

                buffer[i] += sample[1] * Math::DB_MINUS18;
            }

            //write to buffer:

            buffer[i] = oscillatorOutputs[0][0] * Math::DB_MINUS18;

            //buffer[i] = sample.sum() * gateSmoother.process(gate);
        }
    }

    void FormaSynth::handleMidiEvent (Control::Midi::Message message)
    {
        if (message.isAllNotesOff())
        {
            return;
        }

        if (message.isNoteOnOrOff())
        {
            int noteNumber = message.data1;

            constexpr int lowestNote = Ath::Math::C2_MIDI_NOTE_NUMBER;
            constexpr int highestNote = lowestNote + 61;

            if ((noteNumber >= lowestNote) && (noteNumber < highestNote))
            {
                int note = noteNumber - lowestNote;

                // fire midi events here
                if (message.isNoteOn())
                {

                }
                else if (message.isNoteOff())
                {
                    for (int i = 0; i < 6; i++)
                    {

                    }
                }
            }

            return;
        }
    }

    void FormaSynth::setParameterFlute16 (float x) 
    { 
        parameterSmootherFluteStops[0].setTargetValue(std::lerp (Math::DB_MINUS60, 1.0f, x)); 
    }

    void FormaSynth::setParameterFlute8 (float x) 
    { 
        parameterSmootherFluteStops[1].setTargetValue(std::lerp (Math::DB_MINUS60, 1.0f, x)); 
    }

    void FormaSynth::setParameterFlute4 (float x) 
    { 
        parameterSmootherFluteStops[2].setTargetValue(std::lerp (Math::DB_MINUS48, 1.0f, x)); 
    }
    
    void FormaSynth::setParameterFlute2 (float x) 
    { 
        parameterSmootherFluteStops[3].setTargetValue(std::lerp (Math::DB_MINUS48, 1.0f, x)); 
    }

    void FormaSynth::setParameterFlute5 (float x) 
    { 
        parameterSmootherFluteStops[4].setTargetValue(std::lerp (Math::DB_MINUS48, 1.0f, x));
    };

    void FormaSynth::setParameterFlute1 (float x) 
    { 
        parameterSmootherFluteStops[5].setTargetValue(std::lerp (Math::DB_MINUS30, 1.0f, x));
    };

}

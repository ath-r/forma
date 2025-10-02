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
            Simd::float8 baseFrequency = Math::noteToFrequency(float(Math::C1_MIDI_NOTE_NUMBER + i) + oscillatorTuningError[i % 12] * 0.01f);
            Simd::float8 multipliers = {
                1.0f, 
                2.0f, 
                4.0f, 
                8.0f, 
                16.0f, 
                32.0f, 
                64.0f, 
                128.0f,
            };
            Simd::float8 frequencies = baseFrequency * multipliers;
            
            auto& osc = oscillators[i];
            osc.setContext(context);
            osc.setFrequency(frequencies);

            auto& osc2 = oscillators2[i];
            osc2.setContext(context);
            osc2.setFrequency(frequencies * 16.0f);
        }

        for (int i = 0; i < KEY_NUMBER; i++)
        {
            keyswitches[i].setContext(context);
            keyswitches[i].init(i);
        }

        for (auto& filterBank : filterBanks) filterBank.setContext(context);
        
        float freq = Math::noteToFrequency(Math::C1_MIDI_NOTE_NUMBER);
        filterBanks[0].setFrequency(freq);
        filterBanks[1].vmul = {1.0f, 2.0f, 2.828f, 4.0f, 5.656f, 8.0};
        filterBanks[1].setFrequency(freq *= 1.4142f);
        filterBanks[2].vmul = {1.0f, 1.4142f, 2.0f, 2.828f, 4.0f, 5.656f};
        filterBanks[2].setFrequency(freq *= 2.0f);
        filterBanks[3].vmul = filterBanks[2].vmul;
        filterBanks[3].setFrequency(freq *= 2.0f);
        filterBanks[4].vmul = filterBanks[2].vmul;
        filterBanks[4].setFrequency(freq *= 2.0f);
        filterBanks[5].vmul = filterBanks[2].vmul;
        filterBanks[5].setFrequency(freq *= 2.0f);

        for (int i = 0; i < KEY_NUMBER; i++)
        {
            const float basefreq = Math::noteToFrequency(Math::C1_MIDI_NOTE_NUMBER + i);
            Simd::float8 freqs = { 1.0f, 2.0f, 4.0f, 8.0f, 3.0f, 10.0f, 1.0f, 1.0f };
            freqs *= basefreq;

            if (i < 6) prefilterGains[i] = filterBanks[0].getAttenutation(freqs) * Math::DB_PLUS1;
            else if (i < 18) prefilterGains[i] = filterBanks[1].getAttenutation(freqs);
            else if (i < 30) prefilterGains[i] = filterBanks[2].getAttenutation(freqs);
            else if (i < 42) prefilterGains[i] = filterBanks[3].getAttenutation(freqs) * Math::DB_MINUS2;
            else if (i < 54) prefilterGains[i] = filterBanks[4].getAttenutation(freqs) * Math::DB_MINUS2;
            else prefilterGains[i] = filterBanks[5].getAttenutation(freqs) * Math::DB_MINUS3;
        }

        hum.setContext(context);

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

            //process oscillators:

            //this generates all octaves for each key
            //oscillatorOutputs[0] = {C1, C2, C3, C4, C5, C6, C7, C8}
            //...
            //oscillatorOutputs[12] = {C2, C3, C4, C5, C6, C7, C8, C9}
            //...
            buffer[i] = 0.0f;
            for (int n = 0; n < OSC_NUMBER; n++)
            {
                auto sample = oscillators[n].processSample();
                auto sample2 = oscillators2[n].processSample();
                oscillatorOutputs[n] = sample;
                oscillatorOutputs[n + 48] = sample2;

                oscillatorOutputs[n] = Simd::ternary(sample, Simd::permute(sample2, Simd::perm4), Simd::mask4);
                oscillatorOutputs[n + 12] = Simd::ternary(Simd::permute(sample, Simd::perm1), Simd::permute(sample2, Simd::perm5), Simd::mask3);
                oscillatorOutputs[n + 24] = Simd::ternary(Simd::permute(sample, Simd::perm2), Simd::permute(sample2, Simd::perm6), Simd::mask2);
                oscillatorOutputs[n + 36] = Simd::ternary(Simd::permute(sample, Simd::perm3), Simd::permute(sample2, Simd::perm7), Simd::mask1);
                
                oscillatorOutputs[n + 60] = Simd::permute(sample2, Simd::perm1);
            }

            //this permutes the cells so the input to each keyswitch gets the signals it needs
            //first 4 cells are octaves, 5th and 6th are mutations
            //keyswitchInputs[n] = {16′, 8′, 4′, 2′, 5⅓′, 1⅗′, 0, 0}
            //e.g. keyswitchInputs[0] = {C1, C2, C3, C4, G2, E4, 0, 0}
            Simd::int8 maskNasat = {0, 0, 0, 0, Simd::m1, 0, 0, 0};
            Simd::int8 maskTerz = {0, 0, 0, 0, 0, Simd::m1, 0, 0};

            Simd::float8 bleed = 0.0f;
            Simd::float8 bleedTerz = 0.0f;
            for (int n = 0; n < KEY_NUMBER; n++)
            {
                auto prinzipal = oscillatorOutputs[n] & Simd::mask4;
                auto nasat = Simd::permute(oscillatorOutputs[n + 7], Simd::perm5) & maskNasat;
                auto terz = Simd ::permute(oscillatorOutputs[n + 4], Simd::perm6) & maskTerz;

                keyswitchInputs[n] = prinzipal + nasat + terz;
                keyswitchOutputs[n] = keyswitchInputs[n] * (keyswitches[n].processSample() + Math::DB_MINUS72);

                bleed += keyswitchInputs[n];
                bleedTerz += (keyswitchOutputs[n] & maskTerz) * (float(n) / KEY_NUMBER);
            }

            //each filterbank gets the portion of keyboard it's responsible for
            for (auto& x : filterBankInputs) x = 0.0f;
            for (int n = 0; n < 6; n++) filterBankInputs[0] += keyswitchOutputs[n] * prefilterGains[n];
            for (int n = 6; n < 18; n++) filterBankInputs[1] += keyswitchOutputs[n] * prefilterGains[n];
            for (int n = 18; n < 30; n++) filterBankInputs[2] += keyswitchOutputs[n] * prefilterGains[n];
            for (int n = 30; n < 42; n++) filterBankInputs[3] += keyswitchOutputs[n] * prefilterGains[n];
            for (int n = 42; n < 54; n++) filterBankInputs[4] += keyswitchOutputs[n] * prefilterGains[n];
            for (int n = 54; n < 61; n++) filterBankInputs[5] += keyswitchOutputs[n] * prefilterGains[n];

            Simd::float8 sum = 0.0f;
            for (int n = 0; n < 6; n++)
            {
                sum += filterBanks[n].process(filterBankInputs[n]);
            }            
            auto x = (sum + bleed * Math::dB(-72)) * 0.015625f;
            auto sumClipped = filterClipper.process(x  * 0.33333f) * 3.0f;
            auto sumCurved = filterNonlinearity.process(sumClipped) * 64.0f;

            buffer[i] = (sumCurved * parameterFluteStops + hum.process() * Math::DB_MINUS48 + bleedTerz * Math::DB_MINUS36).sum() * Math::DB_MINUS18 / 6.0f;
        }
    }

    void FormaSynth::handleMidiEvent (Control::Midi::Message message)
    {
        if (message.isControlChange())
        {
            auto cc = static_cast<Midi::MessageCC>(message);
            float value = float(cc.value) / 127.0f;
            
            switch (cc.cc)
            {
                case 0x0c: setParameterFlute16(value); break;
                case 0x0d: setParameterFlute8(value); break;
                case 0x0e: setParameterFlute5(value); break;
                case 0x0f: setParameterFlute4(value); break;
                case 0x10: setParameterFlute2(value); break;
                case 0x11: setParameterFlute1(value); break;
                case 0x4c: setParameterDrive(value); break;
                default: break;
            }

            return;
        }

        if (message.isAllNotesOff())
        {
            for (auto& keyswitch : keyswitches)
            {
                keyswitch.handleNoteOff(Midi::MessageNoteOff());
            }
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
                    keyswitches[note].handleNoteOn(static_cast<Midi::MessageNoteOn>(message));
                }
                else if (message.isNoteOff())
                {
                    keyswitches[note].handleNoteOff(static_cast<Midi::MessageNoteOff>(message));
                }
            }

            return;
        }
    }

    void FormaSynth::setParameterFlute16 (float x) 
    { 
        parameters[F16] = x;
        parameterFluteStopsInputs[0] = std::lerp (Math::DB_MINUS54, 1.0f, x);
        parameterFluteStops = parameterFluteStopsInputs.data();
    }

    void FormaSynth::setParameterFlute8 (float x) 
    { 
        parameters[F8] = x;
        parameterFluteStopsInputs[1] = std::lerp (Math::DB_MINUS48, 1.0f, x);
        parameterFluteStops = parameterFluteStopsInputs.data();
    }

    void FormaSynth::setParameterFlute4 (float x) 
    { 
        parameters[F4] = x;
        parameterFluteStopsInputs[2] = std::lerp (Math::DB_MINUS42, 1.0f, x);
        parameterFluteStops = parameterFluteStopsInputs.data(); 
    }
    
    void FormaSynth::setParameterFlute2 (float x) 
    { 
        parameters[F2] = x;
        parameterFluteStopsInputs[3] = std::lerp (Math::DB_MINUS48, 1.0f, x);
        parameterFluteStops = parameterFluteStopsInputs.data(); 
    }

    void FormaSynth::setParameterFlute5 (float x) 
    { 
        parameters[F5] = x;
        parameterFluteStopsInputs[4] = std::lerp (Math::DB_MINUS54, 1.0f, x);
        parameterFluteStops = parameterFluteStopsInputs.data();
    };

    void FormaSynth::setParameterFlute1 (float x) 
    {
        parameters[F1] = x;
        parameterFluteStopsInputs[5] = std::lerp (Math::DB_MINUS48, 1.0f, x);
        parameterFluteStops = parameterFluteStopsInputs.data();
    }

    void FormaSynth::setParameterDrive (float x) 
    {
        parameters[DRIVE] = x;
        parameterDriveGain = 1.0f + x * 16.0f;
    };
}

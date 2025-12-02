#include "FormaSynth.h"
#include "PluginParameters.h"
#include "control/Midi.h"
#include "math/Complex.h"
#include "math/Conversion.h"
#include "math/Simd.h"
#include <cmath>

namespace Ath::Forma
{

    FormaSynth::FormaSynth()
    {
        //initialize arrays
        oscillatorOutputs.fill(0.0f);
        needleContactInputs.fill(0.0f);
        needleContactOutputs.fill(0.0f);
        prefilterGains.fill(1.0f);
        filterBankInputs.fill(0.0f);
        parameterFluteStopsInputs.fill(0.0f);

        //initialize to default parameters' values
        for (int i = 0; i < PARAM_COUNT; i++)
        {
            parameters[i].touchSilently(ParametersByID[i].def);
        }

        //initialize oscillators
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
            osc.setFrequency(frequencies);

            auto& osc2 = oscillators2[i];
            osc2.setFrequency(frequencies * 16.0f);
        }

        // initialize keyswitches
        for (int i = 0; i < KEY_NUMBER; i++)
        {
            needleContacts[i].init(i);
        }

        // initialize filterbanks
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

        // initialize voicing to even out filter attenuation
        for (int i = 0; i < KEY_NUMBER; i++)
        {
            const float basefreq = Math::noteToFrequency(Math::C1_MIDI_NOTE_NUMBER + i);
            Simd::float8 freqs = { 1.0f, 2.0f, 4.0f, 8.0f, 3.0f, 10.0f, 1.0f, 1.0f };
            freqs *= basefreq;

            // I KNOW that this can be expressed with a very simple formula, but let's keep it like that
            // since it's more readable and can be flexibly changed in case filter bank distribution changes
            int filterBankIndex;
            if (i < 6) filterBankIndex = 0;
            else if (i < 18) filterBankIndex = 1;
            else if (i < 30) filterBankIndex = 2; 
            else if (i < 42) filterBankIndex = 3;
            else if (i < 54) filterBankIndex = 4;
            else filterBankIndex = 5;

            auto& filterBank = filterBanks[filterBankIndex];
            Math::complex<Simd::float8> transfer = { 0.0f, 0.0f };
            
            // This algorithm samples the transfer function of a given filterbank 
            // for the first OVERTONES_TO_SAMPLE overtones of a given tone
            // Sum of these TFs is then used to determine how much this tone needs to be amplified
            // to compensate for filters' attenuation
            constexpr int OVERTONES_TO_SAMPLE = 10;
            for (int n = 1; n < OVERTONES_TO_SAMPLE; n += 2)
            {
                //TODO: normal division/multiplication by scalar for complex numbers
                auto t = filterBank.getTransfer(freqs * float(n));
                t.re /= float(n);
                t.im /= float(n);

                transfer += t;
            }
            prefilterGains[i] = Simd::rmag(transfer.re, transfer.im) * std::lerp(1.0f, Math::dB(-6), float(i) / 61.0f);
        }
        
        percussionGenerator.setTime(getParameter(PERC_TIME).value);
        gateSmoother.setTime(0.001f);
    }

    void FormaSynth::setContext (Dsp::Context context)
    {
        for (int i = 0; i < OSC_NUMBER; i++)
        {
            oscillators[i].setContext(context);
            oscillators2[i].setContext(context);
        }

        for (int i = 0; i < KEY_NUMBER; i++)
        {
            needleContacts[i].setContext(context);
        }

        for (auto& filterBank : filterBanks) filterBank.setContext(context);

        hum.setContext(context);
        filterTone.setContext(context);

        percussionGenerator.setContext(context);
        gateSmoother.setContext(context);
    }

    void FormaSynth::processBlock (float* buffer, int numberOfSamples)
    {
        if (voiceCount > 0)
        {
            gate = 1.0f;
            gateSmoother.setTime(0.0001f);
        }
        else
        {
            gate = 0.0f;
            gateSmoother.setTime(5.0f);
        }

        parameterFluteStops = parameterFluteStopsInputs.data();

        if (percEnabled) parameterPercStops = parameterPercStopsInputs.data();
        else parameterPercStops = Math::dB(-50);

        for (int i = 0; i < numberOfSamples; i++)
        {
            //process oscillators:

            //this generates all octaves for each key
            //oscillatorOutputs[0] = {C1, C2, C3, C4, C5, C6, C7, C8}
            //...
            //oscillatorOutputs[12] = {C2, C3, C4, C5, C6, C7, C8, C9}
            //...

            for (auto& osc : oscillators) osc.processSample();
            for (auto& osc : oscillators2) osc.processSample();
            for (auto& needle : needleContacts) needle.processSample();
            percussionGenerator.processSample();

            for (int n = 0; n < OSC_NUMBER; n++)
            {
                auto sample = oscillators[n].last();
                auto sample2 = oscillators2[n].last();
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
            //needleContactInputs[n] = {16′, 8′, 4′, 2′, 5⅓′, 1⅗′, 0, 0}
            //e.g. needleContactInputs[0] = {C1, C2, C3, C4, G2, E4, 0, 0}
            Simd::int8 maskNasat = {0, 0, 0, 0, Simd::m1, 0, 0, 0};
            Simd::int8 maskTerz = {0, 0, 0, 0, 0, Simd::m1, 0, 0};

            Simd::float8 bleed = 0.0f;
            Simd::float8 bleedTerz = 0.0f;
            for (int n = 0; n < KEY_NUMBER; n++)
            {
                auto prinzipal = oscillatorOutputs[n] & Simd::mask4;
                auto nasat = Simd::permute(oscillatorOutputs[n + 7], Simd::perm5) & maskNasat;
                auto terz = Simd::permute(oscillatorOutputs[n + 4], Simd::perm6) & maskTerz;

                needleContactInputs[n] = prinzipal + nasat + terz;
                needleContactOutputs[n] = needleContactInputs[n] * needleContacts[n].last();

                auto bleedAttenuation = (float(n) / float(KEY_NUMBER));
                bleedAttenuation = Math::ipow(bleedAttenuation, 4);
                // bleed from ungated oscillators i.e. it's present even when no keys are pressed
                bleed += needleContactInputs[n] * bleedAttenuation;
                // in the real organ unfiltered 5'1/3 stop bleeds through, the higher the key the more bleed there is
                bleedTerz += (needleContactOutputs[n] & maskTerz) * bleedAttenuation;
            }

            //each filterbank gets the portion of keyboard it's responsible for
            for (auto& x : filterBankInputs) x = 0.0f;
            for (int n = 0; n < 6; n++) filterBankInputs[0] += needleContactOutputs[n] * prefilterGains[n];
            for (int n = 6; n < 18; n++) filterBankInputs[1] += needleContactOutputs[n] * prefilterGains[n];
            for (int n = 18; n < 30; n++) filterBankInputs[2] += needleContactOutputs[n] * prefilterGains[n];
            for (int n = 30; n < 42; n++) filterBankInputs[3] += needleContactOutputs[n] * prefilterGains[n];
            for (int n = 42; n < 54; n++) filterBankInputs[4] += needleContactOutputs[n] * prefilterGains[n];
            for (int n = 54; n < 61; n++) filterBankInputs[5] += needleContactOutputs[n] * prefilterGains[n];

            //process filterbanks
            Simd::float8 sum = 0.0f;
            for (int n = 0; n < 6; n++)
            {
                sum += filterBanks[n].process(filterBankInputs[n]);
            }            

            //filter nonlinearity:
            static Simd::float8 postNonlinearityGain = 128.0f;
            static Simd::float8 preNonlinearityGain = 1.0f / 128.0f;
            auto filterAmpIn = (sum + bleed * keyboardBleedGain) * preNonlinearityGain;

            //hard clipper (usually won't be reached):
            //auto sumClipped = filterClipper.process(x  * 0.33333f) * 3.0f;

            //soft clipper:
    //real clipping won't occur in normal circumstances, because we attenuate the signal by a factor of 64
    //so it doesn't clip even with all keys pressed
    //since the transfer curve is very nonlinear, intermodulation harmonics will be present
    //even with a low input signal amplitude
            auto filterAmpOut = filterNonlinearity.process(filterAmpIn) * postNonlinearityGain;
            auto fluteOut = filterAmpOut * parameterFluteStops;

            auto percIn = (filterAmpOut * parameterPercStops * preNonlinearityGain).sum();
            auto percOut = percusionNonlinearity.process(percIn) * 128.0f * percussionGenerator.last();

            //bleeds:
            auto outBleed = bleedTerz * terzBleedGain + bleed * keyboardBleedGain;

            //tone knob filter:
            auto toneIn = fluteOut + outBleed;
            buffer[i] = toneIn.sum() + percOut;
        }

        for (int i = 0; i < numberOfSamples; i++) buffer[i] += hum.process() * noiseFloorGain;

        // second loop for effects that come after flattening the SIMD into float
        for (int i = 0; i < numberOfSamples; i++)
        {
            auto toneOut = filterTone.process(buffer[i]);

            //output nonlinearity
            static float postNonlinearityGain = 256.0f;
            static float preNonlinearityGain = 1.0f / 256.0f;
            auto ampIn = toneOut * (preNonlinearityGain / 6.0f);
            auto ampOut = outputNonlinearity.process(ampIn) * (postNonlinearityGain * 6.0f);

            //lastly, attenuate output signal by 18dB and a factor of 6 (because there are 6 stops)
            buffer[i] = float(ampOut) * Math::DB_MINUS18 / 6.0f * gateSmoother.process(gate);
        }

        parameters[PERC_CV].touch(percussionGenerator.last() * percussionGenerator.getGate());
    }

    void FormaSynth::setParameter (int parameterIndex, float x, bool touch) 
    {
        if (touch) parameters[parameterIndex].touch(x);
        else parameters[parameterIndex].touchSilently(x);

        const bool booleanValue = x >= 0.5f;
        bool updatePercussionPreset = false;

        switch(parameterIndex)
        {
            case F16: parameterFluteStopsInputs[0] = std::lerp (Math::DB_MINUS54, 1.0f, x); break;
            case F8: parameterFluteStopsInputs[1] = std::lerp (Math::DB_MINUS48, 1.0f, x); break;
            case F4: parameterFluteStopsInputs[2] = std::lerp (Math::DB_MINUS42, 1.0f, x); break;
            case F2: parameterFluteStopsInputs[3] = std::lerp (Math::DB_MINUS48, Math::DB_MINUS3, x); break;
            case F5: parameterFluteStopsInputs[4] = std::lerp (Math::DB_MINUS54, 1.0f, x); break;
            case F1: parameterFluteStopsInputs[5] = std::lerp (Math::DB_MINUS48, Math::DB_MINUS3, x); break;

            case TONE: filterTone.setCutoffFrequency(Math::logerp2(500.0f, 15000.0f, x)); break;

            case PERC_ON: percEnabled = booleanValue; break;
            case P16: parameterPercStopsInputs[0] = std::lerp (Math::DB_MINUS54, 1.0f, x); break;
            case P8: parameterPercStopsInputs[1] = std::lerp (Math::DB_MINUS54, 1.0f, x); break;
            case P4: parameterPercStopsInputs[2] = std::lerp (Math::DB_MINUS54, 1.0f, x); break;
            case P2: parameterPercStopsInputs[3] = std::lerp (Math::DB_MINUS54, Math::DB_MINUS3, x); break;
            case P5: parameterPercStopsInputs[4] = std::lerp (Math::DB_MINUS54, 1.0f, x); break;
            case P1: parameterPercStopsInputs[5] = std::lerp (Math::DB_MINUS54, Math::DB_MINUS3, x); break;
            
            case PERC_TIME: percussionGenerator.setTime(std::lerp(0.1f, 10.0f, x * x)); break;
            case PERC_CRESC: percussionGenerator.setCrescendo(x > 0.5f); break;

            case PERC_SOFT:
            case PERC_SPEED:
            case PERC_HARMONIC: updatePercussionPreset = true; break;

            case BLEED_KEYBOARD: keyboardBleedGain = Math::decibelsToAmplitude(x); break;
            case BLEED_TERZ: terzBleedGain = Math::decibelsToAmplitude(x); break;
            case NOISE_FLOOR: noiseFloorGain = Math::decibelsToAmplitude(x); break;
        }

        if (updatePercussionPreset)
        {
            const bool softMode = getParameter(PERC_SOFT).value >= 0.5f;
            const bool speedMode = getParameter(PERC_SPEED).value >= 0.5f;
            const bool harmonicMode = getParameter(PERC_HARMONIC).value >= 0.5f;

            setParameter(PERC_TIME, speedMode ? 0.1f : 0.2f, true);
            setParameter(P16, 0.0f, true);
            setParameter(P8, !harmonicMode, true);
            setParameter(P5, harmonicMode, true);
            setParameter(P4, !harmonicMode && softMode, true);
            setParameter(P2, !harmonicMode && softMode, true);
            setParameter(P1, harmonicMode && softMode, true);

            setParameter(PERC_CRESC, 0.0f, true);
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
                case 0x0c: setParameter(F16, value, true); break;
                case 0x0d: setParameter(F8, value, true); break;
                case 0x0e: setParameter(F5, value, true); break;
                case 0x0f: setParameter(F4, value, true); break;
                case 0x10: setParameter(F2, value, true); break;
                case 0x11: setParameter(F1, value, true); break;
                case 0x0b:
                case 0x14: setParameter(TONE, value, true); break;
                case 0x42: setParameter(PERC_ON, value, true); break;
                case 0x46: setParameter(PERC_SOFT, value, true); break;
                case 0x47: setParameter(PERC_SPEED, value, true); break;
                case 0x48: setParameter(PERC_HARMONIC, value, true); break;
                default: break;
            }

            return;
        }

        if (message.isAllNotesOff())
        {
            for (auto& contact : needleContacts)
            {
                contact.handleNoteOff(Midi::MessageNoteOff());
                percussionGenerator.handleNoteOff(Midi::MessageNoteOff());
                voiceCount = 0;
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
                auto& needle = needleContacts[note];

                // fire midi events here
                if (message.isNoteOn())
                {
                    auto m = static_cast<Midi::MessageNoteOn>(message);

                    if (percEnabled && !needle.isActive() && (voiceCount == 0)) percussionGenerator.handleNoteOn(m);

                    if (!needle.isGateOn())
                    {
                        needle.handleNoteOn(m);
                        voiceCount += 1;
                    }
                }

                else if (message.isNoteOff())
                {
                    if (needle.isGateOn())
                    {
                        auto m = static_cast<Midi::MessageNoteOff>(message);
                        needle.handleNoteOff(m);

                        voiceCount -= 1;
                        if (voiceCount == 0) percussionGenerator.handleNoteOff(m);
                    }
                }
            }

            return;
        }
    }
    
}

// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: PulseShaper.h
// Description: The pulse shaping filter following the TR-808's trigger
//              generator circuit.
// ===========================================================================
#pragma once

#include "IIRAnalogFilter.h"
#include "TR808Components.h"

class PulseShaperShelfFilter : public IIRAnalogFilter
{
public:
    PulseShaperShelfFilter(
        int sampleRate,
        TR808Components& components);

protected:
    TR808Components& components;

    void calculateAnalogCoefficients() override;

};

class PulseShaper
{
// Wrapper class is simply to apply nonlinearity -- in retrospect this could
// also have been achieved by overriding the process(double) method of
// IIRAnalogFilter.
public:
    PulseShaper(int sampleRate, TR808Components& components);
    double process(double x);

private:
    PulseShaperShelfFilter filter;
};

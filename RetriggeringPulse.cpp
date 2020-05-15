// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: RetriggeringPulse.cpp
// Description: Implements the retriggering pulse filter following the TR-808
//              envelope generator.
// ===========================================================================
#include "RetriggeringPulse.h"
#include "utils.h"

RetriggeringPulseFilter::RetriggeringPulseFilter(
    int sampleRate,
    TR808Components& components)
    : IIRAnalogFilter(sampleRate, 1),
      components(components)
{
    init();
}

void RetriggeringPulseFilter::calculateAnalogCoefficients()
{
    beta[0] = 0;
    beta[1] = components.R161 * components.C39;
    alpha[0] = 1;
    alpha[1] = components.R161 * components.C39;
}

RetriggeringPulse::RetriggeringPulse(
    int sampleRate,
    TR808Components& components)
    : filter(sampleRate, components)
{
}

double RetriggeringPulse::process(double x)
{
    auto y = filter.process(x);
    return diodeNonlinearity(y);
}

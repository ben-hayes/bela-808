// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: PulseShaper.cpp
// Description: The pulse shaping filter following the TR-808's trigger
//              generator circuit.
// ===========================================================================
#include <cmath>

#include "PulseShaper.h"
#include "utils.h"

PulseShaperShelfFilter::PulseShaperShelfFilter(
    int sampleRate,
    TR808Components& components)
    : IIRAnalogFilter(sampleRate, 1),
      components(components)
{
    init();
}

void PulseShaperShelfFilter::calculateAnalogCoefficients()
{
    alpha[0] = components.R162 + components.R163
                / (components.R162 * components.R163 * components.C40);
    alpha[1] = 1.0;
    beta[0] = components.R162
                / (components.R162 * components.R163 * components.C40);
    beta[1] = 1.0;
}

PulseShaper::PulseShaper(int sampleRate, TR808Components& components)
    : filter(sampleRate, components)
{
}

double PulseShaper::process(double x)
{
    auto y = filter.process(x);
    // The nonlinearity (as described in Werner's paper) simple approximates the
    // diode behaviour, in lieu of solving the full ODE.
    return diodeNonlinearity(y);
}

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
    // Actually scaled proportionally into a range more amenable to doubleing
    // point calculations, but the effect is the same.
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
    return diodeNonlinearity(y);
}

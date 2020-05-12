#include "RetriggeringPulse.h"
#include "utils.h"

RetriggeringPulseFilter::RetriggeringPulseFilter(
    int sampleRate,
    TR808Components components)
    : IIRAnalogFilter(sampleRate, 1)
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
    TR808Components components)
    : filter(sampleRate, components)
{
}

double RetriggeringPulse::process(double x)
{
    auto y = filter.process(x);
    return diodeNonlinearity(y);
}

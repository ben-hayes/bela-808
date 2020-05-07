#include "RetriggeringPulse.h"

RetriggeringPulse::RetriggeringPulse(
    int sampleRate,
    TR808Components components)
    : IIRAnalogFilter(sampleRate, 1)
{
    init();
}

void RetriggeringPulse::calculateAnalogCoefficients()
{
    beta[0] = 0;
    beta[1] = components.R161 * components.C39;
    alpha[0] = 1;
    alpha[1] = components.R161 * components.C39;
}

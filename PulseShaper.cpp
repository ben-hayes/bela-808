#include <cmath>

#include "PulseShaper.h"

PulseShaperShelfFilter::PulseShaperShelfFilter(
    int sampleRate,
    PulseShaperComponentValues components)
{
    calculateAnalogCoefficients(components);
    calculateDigitalCoefficients(sampleRate);
}

float PulseShaperShelfFilter::process(float x)
{
    auto y = 0.0f;
    y += b0 * x;
    y += b1 * last_x;
    y += -a1 * last_y;

    last_y = y;
    last_x = x;

    return y;
}

void PulseShaperShelfFilter::calculateAnalogCoefficients(
    PulseShaperComponentValues components)
{
    // Actually scaled proportionally into a range more amenable to floating
    // point calculations, but the effect is the same.
    alpha[0] = components.R162 + components.R163
                / (components.R162 * components.R163 * components.C40);
    alpha[1] = 1.0f;
    beta[0] = components.R162
                / (components.R162 * components.R163 * components.C40);
    beta[1] = 1.0f;
}

void PulseShaperShelfFilter::calculateDigitalCoefficients(int sampleRate)
{
    auto T = 1.0f / sampleRate;

    auto denom = 2.0f * alpha[1] + T * alpha[0];

    a1 = (T * alpha[0] - 2.0f * alpha[1]) / denom;
    b0 = (2.0f * beta[1] + T * beta[0]) / denom;
    b1 = (T * beta[0] - 2.0f * beta[1]) / denom;
}

PulseShaper::PulseShaper(int sampleRate)
    : filter(sampleRate, components)
{
}

float PulseShaper::process(float x)
{
    auto y = filter.process(x);
    return memorylessNonlinearity(y);
}

float PulseShaper::memorylessNonlinearity(float voltage)
{
    if (voltage >= 0.0f) return voltage;
    else return 0.71f * (expf(voltage) - 1.0f);
}

#include "ToneControl.h"

ToneControlLowPassFilter::ToneControlLowPassFilter(
    int sampleRate,
    TR808Components& components)
    : IIRAnalogFilter(sampleRate, 1),
      components(components)
{
    init();
}

double ToneControlLowPassFilter::process(double x, double l)
{
    this->l = l;
    init();
    return IIRAnalogFilter::process(x);
}

void ToneControlLowPassFilter::calculateAnalogCoefficients()
{
    auto Req = components.R172 * components.VR5 * l;
    Req /= components.R172 + components.VR5 * l;
    Req += components.R171;

    beta[0] = 1;
    beta[1] = 0;
    alpha[0] = 1;
    alpha[1] = components.C45 * Req;
}

ToneControlLevelFilter::ToneControlLevelFilter(
    int sampleRate,
    TR808Components& components)
    : IIRAnalogFilter(sampleRate, 1),
      components(components)
{
    init();
}

double ToneControlLevelFilter::process(double x, double m)
{
    this->m = m;
    init();
    return IIRAnalogFilter::process(x);
}

void ToneControlLevelFilter::calculateAnalogCoefficients()
{
    beta[0] = 0;
    beta[1] = components.VR6 * (1 - m) * components.C47;
    alpha[0] = 1;
    alpha[1] = components.VR6 * components.C47;
}

ToneControlHighPassFilter::ToneControlHighPassFilter(
    int sampleRate,
    TR808Components& components)
    : IIRAnalogFilter(sampleRate, 1),
      components(components)
{
    init();
}

void ToneControlHighPassFilter::calculateAnalogCoefficients()
{
    beta[0] = 0;
    beta[1] = components.R177 * components.C49;
    alpha[0] = 1;
    alpha[1] = components.R176 * components.C49;
}

ToneControl::ToneControl(int sampleRate, TR808Components& components)
    : lpf(sampleRate, components),
      level(sampleRate, components),
      hpf(sampleRate, components)
{
}

double ToneControl::process(double x, double l, double m)
{
    auto y = lpf.process(x, l);
    y = level.process(y, m);
    y = hpf.process(y);

    return y;
}

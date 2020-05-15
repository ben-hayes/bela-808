// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: ToneControl.cpp
// Description: Implements tone control filters for output stage of 808 model.
// ===========================================================================
#include "ToneControl.h"

ToneControlLowPassFilter::ToneControlLowPassFilter(
    int sampleRate,
    TR808Components& components)
    : IIRAnalogFilter(sampleRate, 1),
      components(components)
{
}

double ToneControlLowPassFilter::process(double x, double l)
{
    this->l = l; // set variable resistor coefficient
    return IIRAnalogFilter::process(x);
}

void ToneControlLowPassFilter::calculateAnalogCoefficients()
{
    // Req is a helper variable described in the paper
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
}

double ToneControlLevelFilter::process(double x, double m)
{
    this->m = m; // set variable resistor coefficient
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
    // Sequentially pass signal through tone control filters:
    auto y = lpf.process(x, l);
    y = level.process(y, m);
    y = hpf.process(y);

    return y;
}

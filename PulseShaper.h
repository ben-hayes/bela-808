#pragma once

#include "IIRAnalogFilter.h"
#include "TR808Components.h"

struct PulseShaperComponentValues
{
    double C40 = 0.015f;
    double R162 = 4700.0f;
    double R163 = 100000.0f;
};

class PulseShaperShelfFilter : public IIRAnalogFilter
{
public:
    PulseShaperShelfFilter(
        int sampleRate,
        TR808Components components);

protected:
    TR808Components components;

    void calculateAnalogCoefficients() override;

};

class PulseShaper
{
public:
    PulseShaper(int sampleRate, TR808Components components);
    double process(double x);

private:
    TR808Components components;
    PulseShaperShelfFilter filter;
};

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
public:
    PulseShaper(int sampleRate, TR808Components& components);
    double process(double x);

private:
    PulseShaperShelfFilter filter;
};

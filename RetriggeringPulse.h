#pragma once
#include "IIRAnalogFilter.h"
#include "TR808Components.h"

class RetriggeringPulseFilter : public IIRAnalogFilter
{
public:
    RetriggeringPulseFilter(int sampleRate, TR808Components components);

private:
    TR808Components components;

    void calculateAnalogCoefficients();
};

class RetriggeringPulse
{
public:
    RetriggeringPulse(int sampleRate, TR808Components components);
    double process(double x);

private:
    RetriggeringPulseFilter filter;
};

#pragma once
#include "IIRAnalogFilter.h"
#include "TR808Components.h"

class RetriggeringPulse : public IIRAnalogFilter
{
public:
    RetriggeringPulse(int sampleRate, TR808Components components);

private:
    TR808Components components;

    void calculateAnalogCoefficients();
};

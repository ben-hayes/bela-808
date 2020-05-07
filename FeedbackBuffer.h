#pragma once
#include "IIRAnalogFilter.h"
#include "TR808Components.h"

class FeedbackBuffer : public IIRAnalogFilter
{
public:
    FeedbackBuffer(int sampleRate, TR808Components components);

    double process(double x, double k);

protected:
    double k = 1.0;
    
    TR808Components components;

    void calculateAnalogCoefficients() override;
};

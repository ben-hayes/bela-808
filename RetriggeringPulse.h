// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: RetriggeringPulse.h
// Description: Implements the retriggering pulse filter following the TR-808
//              envelope generator.
// ===========================================================================
#pragma once
#include "IIRAnalogFilter.h"
#include "TR808Components.h"

class RetriggeringPulseFilter : public IIRAnalogFilter
{
public:
    RetriggeringPulseFilter(int sampleRate, TR808Components& components);

private:
    TR808Components& components;

    void calculateAnalogCoefficients();
};

class RetriggeringPulse
{
public:
    RetriggeringPulse(int sampleRate, TR808Components& components);
    double process(double x);

private:
    RetriggeringPulseFilter filter;
};

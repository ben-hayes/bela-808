// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: ToneControl.h
// Description: Implements tone control filters for output stage of 808 model.
// ===========================================================================
#pragma once
#include "IIRAnalogFilter.h"
#include "TR808Components.h"

class ToneControlLowPassFilter : public IIRAnalogFilter
{
public:
    ToneControlLowPassFilter(int sampleRate, TR808Components& components);

    double process(double x, double l);

protected:
    double l = 1.0;

    TR808Components& components;

    void calculateAnalogCoefficients() override;
};

class ToneControlLevelFilter : public IIRAnalogFilter
{
public:
    ToneControlLevelFilter(int sampleRate, TR808Components& components);

    double process(double x, double m);

protected:
    double m = 1.0;

    TR808Components& components;

    void calculateAnalogCoefficients() override;
};

class ToneControlHighPassFilter : public IIRAnalogFilter
{
public:
    ToneControlHighPassFilter(int sampleRate, TR808Components& components);

protected:
    TR808Components& components;

    void calculateAnalogCoefficients() override;
};

class ToneControl
{
public:
    ToneControl(int sampleRate, TR808Components& components);

    double process(double x, double l, double m);

protected:
    ToneControlLowPassFilter lpf;
    ToneControlLevelFilter level;
    ToneControlHighPassFilter hpf;
};

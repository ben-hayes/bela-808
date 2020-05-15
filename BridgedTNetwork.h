// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: BridgedTNetwork.h
// Description: The bridged T network at the core of the TR-808's sound
//              generation capabilities.
// ===========================================================================
#pragma once
#include "FeedbackBuffer.h"
#include "IIRAnalogFilter.h"
#include "TR808Components.h"
#include "utils.h"

class BridgedTFilter1 : public IIRAnalogFilter
{
public:
    BridgedTFilter1(int sampleRate, TR808Components& components);

protected:
    TR808Components& components;

    void calculateAnalogCoefficients() override;
};

class BridgedTFilter2 : public IIRAnalogFilter
{
public:
    BridgedTFilter2(int sampleRate, TR808Components& components);

protected:
    TR808Components& components;

    void calculateAnalogCoefficients() override;
};

class BridgedTFilter3 : public IIRAnalogFilter
{
public:
    BridgedTFilter3(int sampleRate, TR808Components& components);

protected:
    TR808Components& components;

    void calculateAnalogCoefficients() override;
};

class ZeroDelayLoop : public IIRAnalogFilter
{
public:
    ZeroDelayLoop(int sampleRate);

protected:
    void calculateAnalogCoefficients() {}
};

class BridgedTNetwork
{
public:
    BridgedTNetwork(int sampleRate, TR808Components& components);

    double process(double pulse, double envelope, double feedbackPot);

private:
    TR808Components& components;
    BridgedTFilter1 Hbt1;
    BridgedTFilter1 Hbt2; // Unfortunately the coefficients given by Werner for
                          // Hbt2 do not result in a transfer function with the
                          // properties he describes, so Hbt1 is re-used, and
                          // the coefficient controlling the feedback variable
                          // resistor is scaled accordingly (as Hbt1 has higher
                          // overall gain)
    BridgedTFilter3 Hbt3;

    FeedbackBuffer Hfb;
    ZeroDelayLoop Hzdl;

    double lastFeedbackPot = -1.0;
};

#pragma once
#include "IIRAnalogFilter.h"
#include "TR808Components.h"
#include "utils.h"

class BridgedTFilter1 : public IIRAnalogFilter
{
public:
    BridgedTFilter1(int sampleRate, TR808Components components);

protected:
    TR808Components components;

    void calculateAnalogCoefficients() override;
};

class BridgedTFilter2 : public IIRAnalogFilter
{
public:
    BridgedTFilter2(int sampleRate, TR808Components components);

protected:
    TR808Components components;

    void calculateAnalogCoefficients() override;
};

class BridgedTFilter3 : public IIRAnalogFilter
{
public:
    BridgedTFilter3(int sampleRate, TR808Components components);

protected:
    TR808Components components;

    void calculateAnalogCoefficients() override;
};

class BridgedTNetwork
{
public:
    BridgedTNetwork(int sampleRate, TR808Components components);

    double process(double pulse, double envelope, double feedback);

private:
    TR808Components components;
    BridgedTFilter1 Hbt1;
    BridgedTFilter2 Hbt2;
    BridgedTFilter3 Hbt3;
};

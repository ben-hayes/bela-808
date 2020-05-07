#pragma once
#include "IIRAnalogFilter.h"
#include "TR808Components.h"
#include "utils.h"

class BridgedTNetwork : public IIRAnalogFilter
{
public:
    BridgedTNetwork(int sampleRate, TR808Components components);

protected:
    TR808Components components;

    void calculateAnalogCoefficients() override;
};

class BridgedTNetwork2 : public IIRAnalogFilter
{
public:
    BridgedTNetwork2(int sampleRate, TR808Components components);

protected:
    TR808Components components;

    void calculateAnalogCoefficients() override;
};

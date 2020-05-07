#include "TriggerCircuit.h"

TriggerCircuit::TriggerCircuit(int sampleRate, double triggerWidthInMs)
{
    triggerWidthInSamples = (int)(0.001 * triggerWidthInMs * sampleRate);
}

void TriggerCircuit::trigger(double accentVoltage)
{
    this->accentVoltage = accentVoltage;
    sampleCounter = 0;
}

double TriggerCircuit::sample()
{
    double out = 0.0;

    if (sampleCounter < triggerWidthInSamples - 1)
    {
        out = accentVoltage;
        sampleCounter += 1;
    }

    return out;
}

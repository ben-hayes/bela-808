#include "TriggerCircuit.h"

TriggerCircuit::TriggerCircuit(int sampleRate, float triggerWidthInMs)
{
    triggerWidthInSamples = (int)(0.001f * triggerWidthInMs * sampleRate);
}

void TriggerCircuit::trigger(float accentVoltage)
{
    this->accentVoltage = accentVoltage;
    sampleCounter = 0;
}

float TriggerCircuit::sample()
{
    float out = 0.0f;

    if (sampleCounter < triggerWidthInSamples - 1)
    {
        out = accentVoltage;
        sampleCounter += 1;
    }

    return out;
}

#pragma once

class TriggerCircuit
{
public:
    TriggerCircuit(int sampleRate, float triggerWidthInMs = 1);

    void trigger(float accentVoltage = 4.0f);
    float sample();

private:
    int triggerWidthInSamples;
    int sampleCounter;
    float accentVoltage;
};

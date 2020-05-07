#pragma once

class TriggerCircuit
{
public:
    TriggerCircuit(int sampleRate, double triggerWidthInMs = 1);

    void trigger(double accentVoltage = 4.0);
    double sample();

private:
    int triggerWidthInSamples;
    int sampleCounter;
    double accentVoltage;
};

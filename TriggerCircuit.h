// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: TriggerCircuit.h
// Description: Generates a simple trigger signal given a desired width and
//              accent.
// ===========================================================================
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

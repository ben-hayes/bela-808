// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: TriggerCircuit.cpp
// Description: Generates a simple trigger signal given a desired width and
//              accent.
// ===========================================================================
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
    // Simple pulse generator -- up if samples haven't elapsed, down if they
    // have.
    double out = 0.0;

    if (sampleCounter < triggerWidthInSamples)
    {
        out = accentVoltage;
        sampleCounter += 1;
    }

    return out;
}

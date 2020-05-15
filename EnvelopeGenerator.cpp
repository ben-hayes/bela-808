// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: EnvelopeGenerator.cpp
// Description: A simple idealised envelope generator used to replace the
//              similar circuit in the TR-808.
// ===========================================================================
#include "EnvelopeGenerator.h"

EnvelopeGenerator::EnvelopeGenerator(
    int sampleRate,
    double upThreshold,
    double downThreshold,
    double attackConstant,
    double downTimeInMs)
    : state(EnvelopeState::Down),
      attackConstant(attackConstant),
      downTimeInSamples(0.001 * downTimeInMs * sampleRate),
      upThreshold(upThreshold),
      downThreshold(downThreshold)
{
}

double EnvelopeGenerator::process(double x)
{
    // According to Werner, the envelope generator "swings quickly up, and
    // doesn’t settle back down to ground until approximately 5 ms after the
    // trigger swings low". So here, we implement a two-state generator with 
    // exponential attack and linear decay, with its state modulated by the 
    // incoming trigger signal using hysteresis.
    double y = 0.0;

    switch (state)
    {
        case EnvelopeState::Up:
            // Make our target the highest trigger seen since this excursion
            // above the threshold:
            if (x > maxTrig) maxTrig = x;
            y = lastY + (maxTrig - lastY) * attackConstant;

            if (x < downThreshold)
            {
                // Change state if we've fallen below the threshold.
                state = EnvelopeState::Down;
                downStepSize = maxTrig / downTimeInSamples;
            }
            break;

        case EnvelopeState::Down: 
            // Take a linear step back to zero
            if (lastY > 0) y = lastY - downStepSize;
            if (y < 0) y = 0.0;

            if (x > upThreshold)
            {
                // Change state if we've crossed the up threshold.
                state = EnvelopeState::Up;
                maxTrig = 0.0;
            }
            break;
    };

    lastY = y;
    return y;
}

// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: FeedbackBuffer.cpp
// Description: The feedback buffer filter connected to the TR-808's
//              bridged T network.
// ===========================================================================
#include <Bela.h>
#include "FeedbackBuffer.h"

FeedbackBuffer::FeedbackBuffer(
    int sampleRate,
    TR808Components& components)
    : IIRAnalogFilter(sampleRate, 1),
      components(components)
{
    init();
}

double FeedbackBuffer::process(double x)
{
    // Overridden process method allows the introduction of op-amp clipping
    return constrain(
        IIRAnalogFilter::process(x),
        -components.upc4558Clipping,
        components.upc4558Clipping);
}

void FeedbackBuffer::calculateAnalogCoefficients()
{
    // The coefficients provided by Werner unfortunately resulted in an unstable 
    // filter with a dramatically different frequency response than is needed.
    // This much simpler shelf filter, parameterised by "k" was empirically
    // determined to function much better for the purpose.
    alpha[0] = 1.0;
    alpha[1] = 1.0;
    beta[0] = 1.0;
    beta[1] = k;

    /*
     * For reference, these were Werner's original coefficients (in terms of
     * component values):
    alpha[0] = components.R164;
    alpha[1] = components.R164
               * (components.R169 + components.VR6 * k)
               * components.C43;
    beta[0] = -components.R169;
    beta[1] = -components.R169 * components.VR6 * k * components.C43;
    */
}

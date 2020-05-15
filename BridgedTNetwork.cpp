// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: BridgedTNetwork.cpp
// Description: The bridged T network at the core of the TR-808's sound
//              generation capabilities.
// ===========================================================================
#include<cmath>
#include<cstdlib>

#include <Bela.h>

#include "BridgedTNetwork.h"

BridgedTFilter1::BridgedTFilter1(
    int sampleRate,
    TR808Components& components)
    : IIRAnalogFilter(sampleRate, 2),
      components(components)
{
    init();    
}

void BridgedTFilter1::calculateAnalogCoefficients()
{
    // Analog coefficient calculations from Hbt1 in Werner's paper:
    beta[2] = components.Reffective
              * components.R167
              * components.C41
              * components.C42;
    beta[1] = components.Reffective * components.C41
              + components.R167 * components.C41
              + components.Reffective * components.C42;
    beta[0] = 1.0;
    alpha[2] = beta[2];
    alpha[1] = components.Reffective * (components.C41 + components.C41);
    alpha[0] = 1.0;
}

BridgedTFilter2::BridgedTFilter2(
    int sampleRate,
    TR808Components& components)
    : IIRAnalogFilter(sampleRate, 2),
      components(components)
{
    init();

}

void BridgedTFilter2::calculateAnalogCoefficients()
{
    // Analog coefficient calculations from Hbt2 in Werner's paper:
    beta[2] = 0.0;
    beta[1] = -components.Rparallel * components.R167 * components.C41;
    beta[0] = 0.0;
    alpha[2] = components.Rparallel 
               * components.R167
               * components.C41
               * components.C42;
    alpha[1] = components.Rparallel
               * components.R170
               * (components.C41 + components.C42);
    alpha[0] = components.Rparallel + components.R170;

    alpha[2] *= 1000000.0; // It is unclear why this scaling factor is
                           // necessary, but it was empirically determined
                           // to provide the correct transfer function.
}

BridgedTFilter3::BridgedTFilter3(
    int sampleRate,
    TR808Components& components)
    : IIRAnalogFilter(sampleRate, 2),
      components(components)
{
    init();
}

void BridgedTFilter3::calculateAnalogCoefficients()
{
    // Analog coefficient calculations from Hbt3 in Werner's paper:
    beta[2] = 0.0;
    beta[1] = -components.Rparallel * components.R167 * components.C41;
    beta[0] = 0.0;
    alpha[2] = components.Rparallel
               * components.R167 
               * components.C41
               * components.C42;
    alpha[1] = components.Rparallel
               * components.R161
               * (components.C41 + components.C42);
    alpha[0] = components.Rparallel + components.R161;

    alpha[2] *= 1000000.0; // It is unclear why this scaling factor is
                           // necessary, but it was empirically determined
                           // to provide the correct transfer function.
}

ZeroDelayLoop::ZeroDelayLoop(int sampleRate)
    : IIRAnalogFilter(sampleRate, 3)
{
}

BridgedTNetwork::BridgedTNetwork(int sampleRate, TR808Components& components)
    : components(components),
      Hbt1(sampleRate, components),
      Hbt2(sampleRate, components),
      Hbt3(sampleRate, components),
      Hfb(sampleRate, components),
      Hzdl(sampleRate)
{
}

double BridgedTNetwork::process(
    double pulse,
    double envelope,
    double feedbackPot)
{
    feedbackPot *= 0.095; // empirically selected for stability

    if (feedbackPot != lastFeedbackPot)
    {
        // let's not waste computation by updating these
        // coefficients more than necessary
        Hfb.setFeedbackCoefficient(feedbackPot);
    }

    // Re-initialise Hbt2 as we're never directly passing samples through it
    // (and therefore initialise is never implicitly called)
    components.updateReffective(envelope);
    Hfb.init();
    Hbt2.init();

    lastFeedbackPot = feedbackPot;

    // Rather than passing samples through the feedback buffer with a single
    // sample delay, here we combine the coefficients of the relevant filters
    // to model the system as a zero-delay loop. First we must pull the
    // coefficients out of two of the filters in the loop:
    auto* b_fb = Hfb.getFIRCoefficients();
    auto* a_fb = Hfb.getIIRCoefficients();
    auto* b_bt = Hbt2.getFIRCoefficients();
    auto* a_bt = Hbt2.getIIRCoefficients();

    // Make a space to put the new coefficients:
    double newb[4];
    double newa[3];

    // Normalising factor (to make sure our first IIR coefficient is 1)
    auto norm = 1.0 - b_bt[0] * b_fb[0];

    // Combine the coefficients (the results of some crunchy algebra):
    newb[0] = 1.0 / norm;
    newb[1] = (a_fb[0] + a_bt[0]) / norm;
    newb[2] = (a_bt[0] * a_fb[0] + a_bt[1]) / norm;
    newb[3] = a_bt[1] * a_fb[0] / norm;

    newa[0] = (
        a_fb[0] + a_bt[0]
        - b_bt[0] * b_fb[1]
        - b_bt[1] * b_fb[0]) / norm;
    newa[1] = (
        a_bt[0] * a_fb[0]
        + a_bt[1]
        - b_bt[1] * b_fb[1]
        - b_bt[2] * b_fb[0]) / norm;
    newa[2] = (a_bt[1] * a_fb[0] - b_bt[2] * b_fb[1]) / norm;

    // Set these as the coefficients of a combined filter:
    Hzdl.setCoefficients(newb, newa);

    // Perform filtering:
    auto pulsePortion = Hbt1.process(pulse);
    auto envPortion = Hbt3.process(envelope);
    auto out = Hzdl.process(pulsePortion + envPortion);

    // Return the sample (clipped by op amp)
    return constrain(
        out,
        -components.upc4558Clipping,
        components.upc4558Clipping);

}

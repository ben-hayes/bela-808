#include<cmath>
#include<cstdlib>

#include <Bela.h>

#include "BridgedTNetwork.h"

BridgedTFilter1::BridgedTFilter1(
    int sampleRate,
    TR808Components components)
    : IIRAnalogFilter(sampleRate, 2),
      components(components)
{
    init();    
}

void BridgedTFilter1::calculateAnalogCoefficients()
{
    beta[2] = components.Reffective * components.R167 * components.C41 * components.C42;
    beta[1] = components.Reffective * components.C41 + components.R167 * components.C41 + components.Reffective * components.C42;
    beta[0] = 1.0;
    alpha[2] = beta[2];
    alpha[1] = components.Reffective * (components.C41 + components.C41);
    alpha[0] = 1.0;
}

BridgedTFilter2::BridgedTFilter2(
    int sampleRate,
    TR808Components components)
    : IIRAnalogFilter(sampleRate, 2),
      components(components)
{
    init();

}

void BridgedTFilter2::calculateAnalogCoefficients()
{
    beta[2] = 0.0;
    beta[1] = -components.Rparallel * components.R167 * components.C41;
    beta[0] = 0.0;
    alpha[2] = components.Rparallel * components.R167 * components.C41 * components.C42;
    alpha[1] = components.Rparallel * components.R170 * (components.C41 + components.C42);
    alpha[0] = components.Rparallel + components.R170;

    alpha[2] *= 1000000.0; // do not know why this works, but it is necessary
                           // to achieve the correct magnitude response. odd.
}

BridgedTFilter3::BridgedTFilter3(
    int sampleRate,
    TR808Components components)
    : IIRAnalogFilter(sampleRate, 2),
      components(components)
{
    init();
}

void BridgedTFilter3::calculateAnalogCoefficients()
{
    beta[2] = 0.0;
    beta[1] = -components.Rparallel * components.R167 * components.C41;
    beta[0] = 0.0;
    alpha[2] = components.Rparallel * components.R167 * components.C41 * components.C42;
    alpha[1] = components.Rparallel * components.R161 * (components.C41 + components.C42);
    alpha[0] = components.Rparallel + components.R161;

    alpha[2] *= 1000000.0; // do not know why this works, but it is necessary
                           // to achieve the correct magnitude response. odd.
}

BridgedTNetwork::BridgedTNetwork(int sampleRate, TR808Components components)
    : components(components),
      Hbt1(sampleRate, components),
      Hbt2(sampleRate, components),
      Hbt3(sampleRate, components)
{
}

double BridgedTNetwork::process(double pulse, double envelope, double feedback)
{
    const double tolerance = 0.0000001;
    //auto noiseFloorTrig = rand() % 100000 / 1000000.0;
    //auto noiseFloorFb = rand() % 100000 / 1000000.0;
    auto pulsePortion = Hbt1.process(pulse);
    auto envPortion = Hbt3.process(envelope);// + noiseFloorTrig;
    auto feedbackPortion = 0.0f;
    if (fabs(feedbackPortion) < tolerance) feedbackPortion = Hbt2.process(pulsePortion + envPortion);
    else feedbackPortion = Hbt2.process(feedback);// + noiseFloorFb;

    return constrain(
        pulsePortion + envPortion + feedbackPortion,
        -components.upc4558Clipping,
        components.upc4558Clipping);
}

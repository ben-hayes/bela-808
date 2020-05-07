#include<cmath>

#include <Bela.h>

#include "BridgedTNetwork.h"

BridgedTNetwork::BridgedTNetwork(
    int sampleRate,
    TR808Components components)
    : IIRAnalogFilter(sampleRate, 2),
      components(components)
{
    init();
}

void BridgedTNetwork::calculateAnalogCoefficients()
{
    beta[2] = 0.0001 * components.Reffective * components.R167 * components.C41 * components.C42;
    beta[1] = 0.0001 * components.Reffective * components.C41 + 0.0001 * components.R167 * components.C41 + 0.0001 * components.Reffective * components.C42;
    beta[0] = 0.0001 * 1.0;
    alpha[2] = beta[2];
    alpha[1] = 0.0001 * components.Reffective * (components.C41 + components.C41);
    alpha[0] = 0.0001 * 1.0;
}

BridgedTNetwork2::BridgedTNetwork2(
    int sampleRate,
    TR808Components components)
    : IIRAnalogFilter(sampleRate, 2),
      components(components)
{
    init();
}

void BridgedTNetwork2::calculateAnalogCoefficients()
{
    beta[2] = 0.0;
    beta[1] = 0.0001 * components.Rparallel * components.R167 * components.C41;
    beta[0] = 0.0;
    alpha[2] = 0.0001 * components.Rparallel * components.R167 * components.C41 * components.C42;
    alpha[1] = 0.0001 * components.Rparallel * components.R170 * (components.C41 + components.C42);
    alpha[0] = 0.0001 * components.Rparallel + 0.0001 * components.R170;
}

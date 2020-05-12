#include <Bela.h>
#include "FeedbackBuffer.h"

FeedbackBuffer::FeedbackBuffer(
    int sampleRate,
    TR808Components components)
    : IIRAnalogFilter(sampleRate, 1),
      components(components)
{
    k = 0.001;
    init();
    printFilter();
}

double FeedbackBuffer::process(double x, double k)
{
    this->k = k;
    init();
    return constrain(
        IIRAnalogFilter::process(x),
        -components.upc4558Clipping,
        components.upc4558Clipping);
}

void FeedbackBuffer::calculateAnalogCoefficients()
{
    alpha[0] = components.R164;
    alpha[1] = components.R164 * (components.R169 + components.VR6 * k) * components.C43;
    beta[0] = -components.R169;
    beta[1] = -components.R169 * components.VR6 * k * components.C43;
}

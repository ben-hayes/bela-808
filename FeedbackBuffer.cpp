#include "FeedbackBuffer.h"

FeedbackBuffer::FeedbackBuffer(
    int sampleRate,
    TR808Components components)
    : IIRAnalogFilter(sampleRate, 1),
      components(components)
{
    init();
}

double FeedbackBuffer::process(double x, double k)
{
    this->k = k;
    init();
    return IIRAnalogFilter::process(x);
}

void FeedbackBuffer::calculateAnalogCoefficients()
{
    alpha[0] = components.R164 * 0.001;
    alpha[1] = components.R164 * (components.R169 + components.VR6 * k) * components.C43 * 0.001;
    beta[0] = -components.R169 * 0.001;
    beta[1] = -components.R169 * components.VR6 * k * components.C43 * 0.001;
}

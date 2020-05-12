#include "EnvelopeGenerator.h"

EnvelopeGenerator::EnvelopeGenerator(
    int sampleRate,
    double upThreshold,
    double downThreshold,
    double attackConstant,
    double downTimeInMs)
    : upThreshold(upThreshold),
      attackConstant(attackConstant),
      downThreshold(downThreshold),
      state(EnvelopeState::Down),
      downTimeInSamples(0.001 * downTimeInMs * sampleRate)
{
}

double EnvelopeGenerator::process(double x)
{
    double y = 0.0;

    switch (state)
    {
        case EnvelopeState::Up:
            if (x > maxTrig) maxTrig = x;
            y = lastY + (maxTrig - lastY) * attackConstant;

            if (x < downThreshold)
            {
                state = EnvelopeState::Down;
                downStepSize = maxTrig / downTimeInSamples;
            }
            break;

        case EnvelopeState::Down: 
            if (lastY > 0) y = lastY - downStepSize;
            if (y < 0) y = 0.0;

            if (x > upThreshold)
            {
                state = EnvelopeState::Up;
                maxTrig = 0.0;
            }
            break;
    };

    lastY = y;
    return y;
}

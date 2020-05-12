#pragma once

class EnvelopeGenerator
{
public:
    EnvelopeGenerator(
        int sampleRate,
        double upThreshold = 0.2,
        double downThreshold = 0.1,
        double attackConstant = 0.998,
        double downTimeInMs = 5.0);
    double process(double x);

protected:
    enum class EnvelopeState
    {
        Up = 0,
        Down
    } state;

    double lastY;
    double attackConstant;
    double downTimeInSamples;

    double envelopeValue;
    double maxTrig;
    double downStepSize;

    double upThreshold;
    double downThreshold;
};

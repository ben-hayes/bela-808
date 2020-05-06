#pragma once

struct PulseShaperComponentValues
{
    float C40 = 0.015f;
    float R162 = 4700.0f;
    float R163 = 100000.0f;
};

class PulseShaperShelfFilter
{
public:
    PulseShaperShelfFilter(
        int sampleRate,
        PulseShaperComponentValues components);
    
    float process(float x);

private:
    float alpha[2];
    float beta[2];

    float a1;
    float b0;
    float b1;

    float last_y = 0.0f;
    float last_x = 0.0f;

    void calculateAnalogCoefficients(PulseShaperComponentValues components);
    void calculateDigitalCoefficients(int sampleRate);

};

class PulseShaper
{
public:
    PulseShaper(int sampleRate);

    float process(float x);
private:
    PulseShaperComponentValues components;
    PulseShaperShelfFilter filter;

    float memorylessNonlinearity(float voltage);
};

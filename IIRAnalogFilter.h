#pragma once

class IIRAnalogFilter
{
public:
    IIRAnalogFilter(int sampleRate, int filterOrder);
    ~IIRAnalogFilter();

    void init();

    double process(double x);

protected:
    int sampleRate;
    double* alpha;
    double* beta;
    double* a;
    double* b;
    double* v_;
    int filterOrder;

    void initialiseBuffers(int filterOrder);
    virtual void calculateAnalogCoefficients() = 0;
    void calculateDigitalCoefficients();
    void printFilter();
};

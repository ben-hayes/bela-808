// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: IIRAnalogFilter.h
// Description: The base class for all IIR filters in the project,
//              implementing 1st and 2nd order bilinear transforms.
// ===========================================================================
#pragma once

class IIRAnalogFilter
{
public:
    IIRAnalogFilter(int sampleRate, int filterOrder);
    ~IIRAnalogFilter();

    void init();

    double process(double x, bool withInit = true);

    void setCoefficients(double* b_new, double* a_new);
    double* getIIRCoefficients();
    double* getFIRCoefficients();
    void printFilter();

protected:
    int sampleRate;
    double* alpha;
    double* beta;
    double* a;
    double* b;
    double* v_;
    int filterOrder;

    void initialiseBuffers(int filterOrder);
    // Responsibility is on the subclass to override this and populate alpha[]
    // and beta[] with s-domain transfer function coefficients:
    virtual void calculateAnalogCoefficients() = 0;
    void calculateDigitalCoefficients();
};

#include <cmath>
#include <Bela.h>
#include "IIRAnalogFilter.h"

IIRAnalogFilter::IIRAnalogFilter(int sampleRate, int filterOrder)
    : sampleRate(sampleRate),
      filterOrder(filterOrder)
{
    initialiseBuffers(filterOrder);
}

IIRAnalogFilter::~IIRAnalogFilter()
{
    delete[] alpha;
    delete[] beta;
    delete[] a;
    delete[] b;
    delete[] v_;
}

void IIRAnalogFilter::init()
{
    calculateAnalogCoefficients();
    calculateDigitalCoefficients();
}

double IIRAnalogFilter::process(double x)
{
    auto y = b[0] * x + v_[0];
    for (int i = 0; i < filterOrder - 1; i++)
    {
        v_[i] = v_[i + 1] + b[i + 1] * x - a[i] * y;
    }
    v_[filterOrder - 1] = b[filterOrder] * x - a[filterOrder - 1] * y;

    return y;
}

void IIRAnalogFilter::initialiseBuffers(int filterOrder)
{
    alpha = new double[filterOrder + 1];
    beta = new double[filterOrder + 1];
    a = new double[filterOrder];
    b = new double[filterOrder + 1];
    v_ = new double[filterOrder];
}

void IIRAnalogFilter::calculateDigitalCoefficients()
{
    auto T = 1.0 / sampleRate;
    if (filterOrder == 1)
    {
        auto denom = 2.0 * alpha[1] + T * alpha[0];

        a[0] = (T * alpha[0] - 2.0 * alpha[1]) / denom;
        b[0] = (2.0 * beta[1] + T * beta[0]) / denom;
        b[1] = (T * beta[0] - 2.0 * beta[1]) / denom;
    } else if (filterOrder == 2)
    {
        auto A = 4.0 * alpha[2] / powf(T, 2.0);
        auto B = 2.0 * alpha[1] / T;
        auto C = 4.0 * beta[2] / powf(T, 2.0);
        auto D = 2.0 * beta[1] / T;
        auto denom = A + B + alpha[0];

        a[0] = 2.0 * (alpha[0] - A) / denom;
        a[1] = (A - B + alpha[0]) / denom;
        b[0] = (C + D + beta[0]) / denom;
        b[1] = 2.0 * (beta[0] - C) / denom;
        b[2] = (C - D + beta[0]) / denom;
    }
}

void IIRAnalogFilter::printFilter()
{
    rt_printf("Alpha: ");
    for (int i = 0; i <= filterOrder; i++)
    {
        rt_printf("%.5f, ", alpha[i]);
    }
    rt_printf("\n");

    rt_printf("Beta: ");
    for (int i = 0; i <= filterOrder; i++)
    {
        rt_printf("%.5f, ", beta[i]);
    }
    rt_printf("\n");

    rt_printf("a: 1.0, ");
    for (int i = 0; i < filterOrder; i++)
    {
        rt_printf("%.5f, ", a[i]);
    }
    rt_printf("\n");

    rt_printf("b: ");
    for (int i = 0; i <= filterOrder; i++)
    {
        rt_printf("%.5f, ", b[i]);
    }
    rt_printf("\n");
}

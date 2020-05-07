#include <cmath>

#include "utils.h"

double parallel(double a, double b)
{
    return 1.0 / (1.0 / a + 1.0 / b);
}

double parallel(double a, double b, double c)
{
    return 1.0 / (1.0 / a + 1.0 / b + 1.0 / c);
}

double diodeNonlinearity(double voltage)
{
    if (voltage >= 0.0) return voltage;
    else return 0.71 * (expf(voltage) - 1.0);
}

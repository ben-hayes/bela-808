// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: utils.cpp
// Description: Provides simple utility functions for working with resistors
//              and diodes.
// ===========================================================================
#include <cmath>

#include "utils.h"

double parallel(double a, double b)
{
    // Implements the parallel operator || (reciprocal of sum of reciprocals)
    // for two operands
    return 1.0 / (1.0 / a + 1.0 / b);
}

double parallel(double a, double b, double c)
{
    // Implements the parallel operator || (reciprocal of sum of reciprocals)
    // for three operands
    return 1.0 / (1.0 / a + 1.0 / b + 1.0 / c);
}

double diodeNonlinearity(double voltage)
{
    // The nonlinearity used to emulate diode behaviour suggested in Werner's
    // paper
    if (voltage >= 0.0) return voltage;
    else return 0.71 * (expf(voltage) - 1.0);
}

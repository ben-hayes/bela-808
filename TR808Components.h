// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: TR808Components.h
// Description: Contains a structure describing all component values from the
//              TR-808 circuit schematic
// ===========================================================================

#pragma once
#include <cmath>

#include <Bela.h>

#include "utils.h"

struct TR808Components
{
    // Resistor values:
    double R161 = 1000000.0;
    double R162 = 4700.0;
    double R163 = 100000.0;
    double R164 = 47000.0;
    double R165 = 47000.0;
    double R166 = 6800.0;
    double R167 = 1000000.0;
    double R169 = 47000.0;
    double R170 = 47000.0;
    double R171 = 220.0;
    double R172 = 10000.0;
    double R176 = 100000.0;
    double R177 = 82000.0;

    // Placeholder parallel resistor values:
    double Reffective = parallel(R161, R165 + R166, R170);
    double Rparallel = parallel(R161, R165 + R166);

    // Capacitor values (note: scaling factor of 1000000.0 is to convert from
    // microfarads to farads)
    double C39 = 0.033 / 1000000.0;
    double C40 = 0.015 / 1000000.0;
    double C41 = 0.015 / 1000000.0;
    double C42 = 0.015 / 1000000.0;
    double C43 = 33.0 /  1000000.0;
    double C45 = 0.1 / 1000000.0;
    double C47 = 0.47 / 1000000.0;
    double C49 = 0.47 / 1000000.0;

    // Variable resistor max resistances:
    double VR5 = 10000.0;
    double VR6 = 500000.0;

    // Op amp voltage limit (modelled as simple digital clipping)
    double upc4558Clipping = 3.0;

    // non-linearity coefficients for calculating pitch sigh with analog
    // modelling
    double alpha = 14.3150;
    double V_0 = -0.5560;
    double m = 0.000014765;

    // base value for calculating pitch sigh with straightforward digital ramp
    double sighPeak = 1000.0;

    void updateReffective(double env)
    {
        // Simple substitute for full effective resistance calculation -- works
        // by using the envelope signal to ramp between the full resistance
        // across R165 + R166 and an empirically selected lower value (chosen
        // to best match the desired attack magnitude response presented in the
        // Werner paper). Whilst the timing characteristics are different, the
        // pitch at least performs a similar movement.
        
        Reffective = R165 + R166 - (R165 + R166 - sighPeak) * env;
        Rparallel = parallel(R161, Reffective);

    }

    void updateReffective(double h1, double h2, double h3)
    {
        // Calculates effective resistance across parallel resistors taking
        // into account leakage across R161. Requires outputs of Hc1, Hc2, and
        // Hc3 transfer functions. Note, this formulation (as given in the
        // Werner paper) is unstable for some inputs.

        auto Vcomm = h1 + h2 + h3;
        auto i_c = -log(1 + exp(-alpha * (Vcomm - V_0))) * m / alpha;
        auto numerator = Vcomm * R166 * (R165 + R166);
        auto denominator = Vcomm * (R165 + R166) - R165 * (Vcomm - R166 * i_c);
        Reffective = numerator / denominator;
    }
};

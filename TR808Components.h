#pragma once
#include <cmath>

#include "utils.h"

struct TR808Components
{
    double alpha = 14.3150;
    double V_0 = -0.5560;
    double m = 0.000014765;
    double R161 = 1000000.0;
    double R162 = 4700.0;
    double R163 = 100000.0;
    double R164 = 47000.0;
    double R165 = 47000.0;
    double R166 = 6800.0;
    double R167 = 1000000.0;
    double R169 = 47000.0;
    double R170 = 47000.0;

    double Reffective = parallel(R161, R165 + R166, R170);
    double Rparallel = parallel(R161, R165 + R166);

    double C39 = 0.033 / 1000000.0;
    double C40 = 0.015 / 1000000.0;
    double C41 = 0.015 / 1000000.0;
    double C42 = 0.015 / 1000000.0;
    double C43 = 33.0 /  1000000000.0; // Why the extra normalising factor? unclear, perhaps nanofarads. In any case, it appears to work.

    double VR6 = 500000.0;

    double upc4558Clipping = 1.0;

    void updateReffective(double h1, double h2, double h3)
    {
        auto Vcomm = h1 + h2 + h3;
        auto i_c = -log(1 + exp(-alpha * (Vcomm - V_0))) * m / alpha;
        auto numerator = Vcomm * R166 * (R165 + R166);
        auto denominator = Vcomm * (R165 + R166) - R165 * (Vcomm - R166 * i_c);
        Reffective = numerator / denominator;
        Rparallel = parallel(R161, Reffective);
    }
};

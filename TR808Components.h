#pragma once
#include "utils.h"

struct TR808Components
{
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
};

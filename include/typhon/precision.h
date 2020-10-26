#ifndef TYPHON_PRECISION_H
#define TYPHON_PRECISION_H

#include <float.h>

namespace typhon {

#define DOUBLE_PRECISION
    typedef double real;
#define REAL_MAX DBL_MAX
#define real_sqrt sqrt
#define real_pow pow
#define real_abs fabs
#define real_sin sin
#define real_cos cos
#define real_exp expf
}

#endif TYPHON_PRECISION_H
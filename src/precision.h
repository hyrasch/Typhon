#ifndef TYPHON_PRECISION_H
#define TYPHON_PRECISION_H

#include <float.h>

namespace typhon {

#define DOUBLE_PRECISION
    typedef double real;
#define REAL_MAX DBL_MAX
#define real_sqrt sqrt
#define real_pow pow
}

#endif TYPHON_PRECISION_H
#ifndef CYCLONE_PRECISION_H
#define CYCLONE_PRECISION_H

#include <float.h>

namespace cyclone {

#define DOUBLE_PRECISION
    typedef double real;
#define REAL_MAX DBL_MAX
#define real_sqrt sqrt
#define real_pow pow
}

#endif CYCLONE_PRECISION_H
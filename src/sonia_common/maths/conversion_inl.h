//
// Created by jeremie on 9/19/16.
//

#ifndef SONIA_COMMON_CONVERSION_INL_H_
#define SONIA_COMMON_CONVERSION_INL_H_

#ifndef SONIA_COMMON_CONVERSION_H_
#error This file may only be included matrix.h
#endif  // SONIA_COMMON_CONVERSION_H_

#include <sonia_common/macros.h>
#include "../macros.h"
#include <math.h>

#define RAD_TO_DEGREE 180.0f/M_PI
#define DEGREE_TO_RAD M_PI/180.0f

namespace sonia_common {
ATLAS_INLINE double RadianToDegree(const double &radian) ATLAS_NOEXCEPT{
  return radian * RAD_TO_DEGREE;
}

ATLAS_INLINE double DegreeToRadian(const double &degree) ATLAS_NOEXCEPT{
  return degree * DEGREE_TO_RAD;
}
}

#endif //SONIA_COMMON_CONVERSION_INL_H_HPP

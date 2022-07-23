/**
 * \file	numbers_inl.h
 * \author	Thibaut Mattio <thibaut.mattio@gmail.com>
 * \date	26/10/2015
 *
 * \copyright Copyright (c) 2015 S.O.N.I.A. All rights reserved.
 *
 * \section LICENSE
 *
 * This file is part of S.O.N.I.A. software.
 *
 * S.O.N.I.A. software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * S.O.N.I.A. software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with S.O.N.I.A. software. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SONIA_COMMON_MATHS_NUMBERS_H_
#error This file may only be included numbers.h
#endif  // SONIA_COMMON_MATHS_NUMBERS_H_

#include <sonia_common/maths/stats.h>
#include <cstdlib>
#include <random>
#include <stdexcept>

namespace sonia_common {

//------------------------------------------------------------------------------
//
template <class Tp_>
ATLAS_ALWAYS_INLINE Tp_ Rand(const Tp_ &low, const Tp_ &high) ATLAS_NOEXCEPT {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<Tp_> dist(low, high);
}

//------------------------------------------------------------------------------
//
template <class Tp_>
ATLAS_ALWAYS_INLINE Tp_ ProbabilityDistribution(const Tp_ &u, const Tp_ &s,
                                                const Tp_ &x) ATLAS_NOEXCEPT {
  return exp(-pow(x - u, 2) / (2 * pow(s, 2))) / sqrt(2 * M_PI * pow(s, 2));
}

//------------------------------------------------------------------------------
//
template <typename Tp_>
ATLAS_ALWAYS_INLINE Tp_ Clamp(const Tp_ &x, const Tp_ &xmin,
                              const Tp_ &xmax) ATLAS_NOEXCEPT {
  return std::min(std::max(x, xmin), xmax);
}

//------------------------------------------------------------------------------
//
template <typename Tp_, typename Up_>
ATLAS_ALWAYS_INLINE Tp_ Clamp(const Tp_ &x, const Up_ &v) {
  static_assert(details::IsIterable<Up_>::value,
                "The data set must be iterable");
  return Clamp(x, Min(v), Max(v));
}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE double Gaussian(const double &x,
                                    const double &v) ATLAS_NOEXCEPT_(false) {
  if (v == 0) {
    throw std::invalid_argument("The variance cannot be null");
  }

  return (1 / sqrt(2 * M_PI * v)) * exp(-pow(x, 2) / (2 * v));
}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE double NormalizedGaussian(const double &x,
                                              const double &v) ATLAS_NOEXCEPT_(false) {
  if (v == 0) {
    throw std::invalid_argument("The variance cannot be null");
  }

  return exp(-pow(x, 2) / (2 * v));
}

//------------------------------------------------------------------------------
//
template <typename Tp_>
ATLAS_ALWAYS_INLINE Tp_ SetPrecision(const Tp_ &v, uint32_t d) ATLAS_NOEXCEPT {
  if (v == 0.0) {
    return static_cast<Tp_>(0.0);
  }
  Tp_ factor = pow(10.0, d - ceil(log10(fabs(v))));
  return round(v * factor) / factor;
}

//------------------------------------------------------------------------------
//
template <typename Tp_>
ATLAS_ALWAYS_INLINE int signum(Tp_ val) ATLAS_NOEXCEPT{
  return (Tp_(0) < val) - (val < Tp_(0));
}


}  // namespace sonia_common

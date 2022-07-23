/**
 * \file	numbers.h
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
#define SONIA_COMMON_MATHS_NUMBERS_H_

#include <sonia_common/macros.h>
#include <sonia_common/maths/trigo.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <limits>
#include <type_traits>

namespace sonia_common {

/**
 * Generate a random value between the two values passed in argument.
 *
 * \param low The minimum value that the random number should take.
 * \param high The maximum value that the random number should take.
 * \return A random number between low and high.
 */
template <class Tp_>
Tp_ Rand(const Tp_ &low = 0, const Tp_ &high = 1) ATLAS_NOEXCEPT;

/**
 *
 */
template <class Tp_>
Tp_ ProbabilityDistribution(const Tp_ &u, const Tp_ &s,
                            const Tp_ &x) ATLAS_NOEXCEPT;

/**
 * Clamp the value x into Xmin and Xmax.
 *
 * \param xmin The minimum value that x must take
 * \param xmax The maximum value that x must take.
 * \return The new value of x.
 */
template <typename Tp_>
Tp_ Clamp(const Tp_ &x, const Tp_ &xmin, const Tp_ &xmax) ATLAS_NOEXCEPT;

/**
 * Clamp x into the data set v.
 *
 * This will actuall call the clamp method with the minimum and the maximum
 * element found on the data set.
 *
 * \return The element x, clamped into the data set v.
 */
template <typename Tp_, typename Up_>
Tp_ Clamp(const Tp_ &x, const Up_ &v);

/**
 * \param v The variance
 */
double Gaussian(const double &x, const double &v) ATLAS_NOEXCEPT_(false);

/**
 * \param v The variance
 */
double NormalizedGaussian(const double &x, const double &v) ATLAS_NOEXCEPT_(false);

/**
 * This function will round the value to n significant digits.
 *
 * This can be usefull when you want to compare two numbers with a
 * certain precision.
 *
 * \param v The number to round.
 */
template <typename Tp_>
Tp_ SetPrecision(const Tp_ &v, uint32_t d) ATLAS_NOEXCEPT;

  /**
   * This function will return the -1,0, or 1 depending on the sign of the provided val;
   * -1 for a number negative, 0 for 0 and 1 for positive number.
   * http://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
   */
template <typename Tp_>
int signum(Tp_ val) ATLAS_NOEXCEPT;
}  // namespace sonia_common

#include <sonia_common/maths/numbers_inl.h>

#endif  // SONIA_COMMON_MATHS_NUMBERS_H_

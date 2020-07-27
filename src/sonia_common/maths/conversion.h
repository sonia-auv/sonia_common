//
// Created by jeremie on 9/19/16.
//
/**
 * \file	matrix.h
 * \author	Thibaut Mattio <thibaut.mattio@gmail.com>
 * \date	20/02/2016
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

#ifndef SONIA_COMMON_CONVERSION_H_
#define SONIA_COMMON_CONVERSION_H_

#include <sonia_common/macros.h>
#include <math.h>

namespace sonia_common {

double RadianToDegree(const double &radian) ATLAS_NOEXCEPT;
double DegreeToRadian(const double &degree) ATLAS_NOEXCEPT;

}  // namespace sonia_common

#include <sonia_common/maths/conversion_inl.h>

#endif //SONIA_COMMON_CONVERSION_H_

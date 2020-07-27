/**
 * \file	trigo_inl.h
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

#ifndef SONIA_COMMON_MATHS_TRIGO_H_
#error This file may only be included trigo.h
#endif  // SONIA_COMMON_MATHS_TRIGO_H_

namespace sonia_common {

//------------------------------------------------------------------------------
//
template <class Tp_>
ATLAS_ALWAYS_INLINE Tp_ DegToRad(const Tp_ &degrees) ATLAS_NOEXCEPT {
  return degrees * (M_PI / 180);
}

//------------------------------------------------------------------------------
//
template <class Tp_>
ATLAS_ALWAYS_INLINE Tp_ RadToDeg(const Tp_ &radians) ATLAS_NOEXCEPT {
  return radians * (180 / M_PI);
}

//------------------------------------------------------------------------------
//
template <typename Tp_>
ATLAS_ALWAYS_INLINE Tp_ AngleDistance(const Tp_ &a1,
                                      const Tp_ &a2) ATLAS_NOEXCEPT {
  Tp_ angle_diff = a1 - a2;

  if (angle_diff < static_cast<Tp_>(-180)) {
    angle_diff += static_cast<Tp_>(-360);
  } else if (angle_diff > static_cast<Tp_>(180)) {
    angle_diff -= static_cast<Tp_>(360);
  }
  return angle_diff;
}

//------------------------------------------------------------------------------
//
template <typename Tp_>
ATLAS_ALWAYS_INLINE Tp_ NormalizeAngle(const Tp_ &angle) ATLAS_NOEXCEPT {
  auto norm = fmod(angle, static_cast<Tp_>(360));
  return norm < 0 ? 360 + norm : norm;
}

}  // namespace sonia_common

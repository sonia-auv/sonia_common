/**
 * \file	matrix_inl.h
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

#ifndef SONIA_COMMON_MATHS_MATRIX_H_
#error This file may only be included matrix.h
#endif  // SONIA_COMMON_MATHS_MATRIX_H_

#include <stdexcept>
#include <eigen3/Eigen/Geometry>
#include "sonia_common/maths/conversion.h"

using std::cos;
using std::sin;
using std::atan2;
using std::asin;
using std::pow;
using std::sqrt;

namespace sonia_common {

//------------------------------------------------------------------------------
//
ATLAS_INLINE Eigen::Quaterniond RotToQuat(const Eigen::Matrix3d &m)
ATLAS_NOEXCEPT {
  return Eigen::Quaterniond(m);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE Eigen::Matrix3d QuatToRot(const Eigen::Quaterniond &b)
ATLAS_NOEXCEPT {

  Eigen::Quaterniond quat = b;
  quat.normalize();
  return quat.toRotationMatrix();
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE Eigen::Matrix3d EulerToRot(const Eigen::Vector3d &vec)
ATLAS_NOEXCEPT {
  // To compile, must separate the construction and the = operator...
  Eigen::Matrix3d m;
  m = Eigen::AngleAxisd(vec.x(), Eigen::Vector3d::UnitZ())
      * Eigen::AngleAxisd(vec.y(), Eigen::Vector3d::UnitY())
      * Eigen::AngleAxisd(vec.z(), Eigen::Vector3d::UnitX());
  return m;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE Eigen::Vector3d RotToEuler(const Eigen::Matrix3d &rot)
ATLAS_NOEXCEPT {
  Eigen::Vector3d vec;
  vec = rot.eulerAngles(2,1,0);
  return vec;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE Eigen::Vector3d QuatToEuler(const Eigen::Quaterniond &b)
ATLAS_NOEXCEPT {
  return sonia_common::RotToEuler(sonia_common::QuatToRot(b));
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE Eigen::Quaterniond EulerToQuat(const Eigen::Vector3d &vec)
ATLAS_NOEXCEPT {
  return sonia_common::RotToQuat(sonia_common::EulerToRot(vec));
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE Eigen::Quaterniond NormalizeQuat(const Eigen::Quaterniond &b)
ATLAS_NOEXCEPT {
  Eigen::Quaterniond quat = b;
  quat.normalize();
  return quat;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE Eigen::Quaterniond ExactQuat(const Eigen::Vector3d &w_ib_b,
                                          double dt,
                                          const Eigen::Quaterniond &b_k) {
  // Euqation 10.24 - Farrell (w_in_b assumed to be 0)
  decltype(w_ib_b) w_bn_b = -w_ib_b;

  // Integration of the quaternion derivative (Equation D.36 - Farrell)
  decltype(w_bn_b) w = 0.5 * w_bn_b * dt;
  double n = w.norm();

  if (std::abs(n) > 1) {
    throw std::runtime_error("Integrated angle too large.");
  }

  auto skew_w = SkewMatrix(w);
  Eigen::Matrix4d w_m;
  w_m(0, 0) = 0;
  w_m(0, 1) = -w(0);
  w_m(0, 2) = -w(1);
  w_m(0, 3) = -w(2);
  w_m(1, 0) = w(0);
  w_m(1, 1) = skew_w(0, 0);
  w_m(1, 2) = skew_w(0, 1);
  w_m(1, 3) = skew_w(0, 2);
  w_m(2, 0) = w(1);
  w_m(2, 1) = skew_w(1, 0);
  w_m(2, 2) = skew_w(1, 1);
  w_m(2, 3) = skew_w(1, 2);
  w_m(3, 0) = w(2);
  w_m(3, 1) = skew_w(2, 0);
  w_m(3, 2) = skew_w(2, 1);
  w_m(3, 3) = skew_w(2, 2);

  double sinw;
  // Handle singularity
  if (n == 0) {
    sinw = 1;
  } else {
    sinw = std::sin(n) / n;
  }

  // Cannot multiply the matrix by a quaternion in Eigen, we are forced to do
  // a conversion before...
  Eigen::Matrix<double, 4, 1> b;
  b(0) = b_k.w();
  b(1) = b_k.x();
  b(2) = b_k.y();
  b(3) = b_k.z();

  // Equation D.36 - Farrell
  Eigen::Matrix<double, 4, 1> exact_b =
      (std::cos(n) * Eigen::Matrix<double, 4, 4>::Identity(4, 4) + sinw * w_m) *
          b;
  return Eigen::Quaterniond(exact_b(0), exact_b(1), exact_b(2), exact_b(3));
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE Eigen::Matrix3d SkewMatrix(const Eigen::Vector3d &v)
ATLAS_NOEXCEPT {
  Eigen::Matrix3d m = Eigen::Matrix3d::Zero();
  m(0, 1) = -v(2);
  m(1, 0) = v(2);
  m(0, 2) = v(1);
  m(2, 0) = -v(1);
  m(1, 2) = -v(0);
  m(2, 1) = v(0);
  return m;
}


}  // namespace sonia_common

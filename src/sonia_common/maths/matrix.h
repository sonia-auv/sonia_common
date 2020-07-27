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

#ifndef SONIA_COMMON_MATHS_MATRIX_H_
#define SONIA_COMMON_MATHS_MATRIX_H_

#include <sonia_common/macros.h>
#include <math.h>
#include <eigen3/Eigen/Eigen>

namespace sonia_common {

/*!
 * Convert a rotation matrix to a quaternion
 */
Eigen::Quaterniond RotToQuat(const Eigen::Matrix3d &m) ATLAS_NOEXCEPT;
/*!
 * Converts a quaternion to a rotation matrix. The function normalize
 * the quaternion before using it.
 */
Eigen::Matrix3d QuatToRot(const Eigen::Quaterniond &m) ATLAS_NOEXCEPT;
/*!
 * Converts a quaternion to an set of euler angles in radians.
 * The output vector is [x, y, z] == [roll, pitch, yaw]
 */
Eigen::Vector3d QuatToEuler(const Eigen::Quaterniond &m) ATLAS_NOEXCEPT;

/*!
 * Converts a euler angles set to a quaternion
 * The input vector is [x, y, z] == [yaw, pitch, roll]
 */
Eigen::Quaterniond EulerToQuat(const Eigen::Vector3d &vec) ATLAS_NOEXCEPT;

/*!
 * Converts a euler angles set to a rotation matrix
 * The input vector is [x, y, z] == [yaw, pitch, roll]
 */
Eigen::Matrix3d EulerToRot(const Eigen::Vector3d &vec) ATLAS_NOEXCEPT;

/*!
 * Converts a rotation matrix to a euler angles set
 * The output vector is [x, y, z] == [roll, pitch, yaw]
 */
Eigen::Vector3d RotToEuler(const Eigen::Matrix3d &rot) ATLAS_NOEXCEPT;

Eigen::Matrix3d SkewMatrix(const Eigen::Vector3d &v) ATLAS_NOEXCEPT;

Eigen::Quaterniond ExactQuat(const Eigen::Quaterniond &m);

Eigen::Quaterniond NormalizeQuat(const Eigen::Quaterniond &b) ATLAS_NOEXCEPT;

}  // namespace sonia_common

#include <sonia_common/maths/matrix_inl.h>

#endif  // SONIA_COMMON_MATHS_MATRIX_H_

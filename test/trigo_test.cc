/**
 * \file	trigo_test.cc
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

#include <gtest/gtest.h>
#include <sonia_common/maths/trigo.h>
#include <sonia_common/maths/numbers.h>

using namespace sonia_common;

TEST(Trigo, angle_conversions) {
  double a1_deg = 0;
  double a1_rad = 0;
  double a2_deg = -572.958;
  double a2_rad = -10;
  double a3_deg = 57.2958;
  double a3_rad = 1.000000358;
  double a4_deg = 7677.63;
  double a4_rad = 134.;

  ASSERT_EQ(SetPrecision(DegToRad(a1_deg), 6), SetPrecision(a1_rad, 6));
  ASSERT_EQ(SetPrecision(RadToDeg(a1_rad), 6), SetPrecision(a1_deg, 6));

  ASSERT_EQ(SetPrecision(DegToRad(a2_deg), 6), SetPrecision(a2_rad, 6));
  ASSERT_EQ(SetPrecision(RadToDeg(a2_rad), 6), SetPrecision(a2_deg, 6));

  ASSERT_EQ(SetPrecision(DegToRad(a3_deg), 6), SetPrecision(a3_rad, 6));
  ASSERT_EQ(SetPrecision(RadToDeg(a3_rad), 6), SetPrecision(a3_deg, 6));

  ASSERT_EQ(SetPrecision(DegToRad(a4_deg), 6), SetPrecision(a4_rad, 6));
  ASSERT_EQ(SetPrecision(RadToDeg(a4_rad), 6), SetPrecision(a4_deg, 6));
}

TEST(Trigo, normalize) {
  ASSERT_EQ(SetPrecision(NormalizeAngle(45.5), 6), SetPrecision(45.5, 6));
  ASSERT_EQ(SetPrecision(NormalizeAngle(-29.), 6), SetPrecision(360.-29, 6));
  ASSERT_EQ(SetPrecision(NormalizeAngle(720.), 6), SetPrecision(0., 6));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

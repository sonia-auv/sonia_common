/**
 * \file	numbers_test.cc
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
#include <sonia_common/maths/numbers.h>

static std::vector<int> v1 = {{
                                  828,
                                  522,
                                  832,
                                  71,
                                  609,
                                  787,
                                  179,
                                  756,
                                  259,
                                  977,
                                  816,
                                  179,
                                  330,
                                  581,
                                  124,
                                  911,
                                  78,
                                  71,
                                  869,
                                  223
                              }};

TEST(StatsTest, clamp) {
  ASSERT_EQ(sonia_common::Clamp(1, 0, 5), 1);
  ASSERT_EQ(sonia_common::Clamp(-1, 0, 3), 0);
  ASSERT_EQ(sonia_common::Clamp(15, 0, 3), 3);

  ASSERT_EQ(sonia_common::Clamp(1.f, 0.f, 3.f), 1);
  ASSERT_EQ(sonia_common::Clamp(-1.f, 0.f, 3.f), 0);
  ASSERT_EQ(sonia_common::Clamp(15, 0, 3), 3);

  ASSERT_EQ(sonia_common::Clamp(543, v1), 543);
  ASSERT_EQ(sonia_common::Clamp(-143, v1), 71);
  ASSERT_EQ(sonia_common::Clamp(14143, v1), 977);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

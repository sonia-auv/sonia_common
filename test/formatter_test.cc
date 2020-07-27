/**
 * \file	Formatter_test.cc
 * \author	Thibaut Mattio <thibaut.mattio@gmail.com>
 * \date	16/11/2015
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
#include <sonia_common/io/formatter.h>

using namespace sonia_common;

TEST(Formatter, all_test) {

  std::string s;

  s = Format("", 12, "null");
  ASSERT_EQ(s, "");

  s = Format("o hai", 42);
  ASSERT_EQ(s, "o hai");

  s = Format("i can has {0}", "Formatting");
  ASSERT_EQ(s, "i can has Formatting");

  s = Format("{0} {0}", 2.5);
  ASSERT_EQ(s, "2.5 2.5");

  s = Format("{0} {1}", 0, 1);
  ASSERT_EQ(s, "0 1");

  s = Format("{1} {0} {1}", "right", "left");
  ASSERT_EQ(s, "left right left");

  s = Format("{0} {1} {2} {1} {2} {3}", 0, 1, 2, 3);
  ASSERT_EQ(s, "0 1 2 1 2 3");
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


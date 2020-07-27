/**
 * \file	time_test.cc
 * \author	Thibaut Mattio <thibaut.mattio@gmail.com>
 * \date	28/06/2015
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
#include <math.h>
#include <sonia_common/sys/fsinfo.h>

TEST(FSInfo, blockSize) {
  auto block_size = sonia_common::BlockSize();

  ASSERT_GE(block_size, 512);        // A block can not be less that 512B
  ASSERT_LE(block_size, 64 * 1024);  // A block can not be less that 64kB
}

TEST(FSInfo, freeSpace) {
  auto free_space = static_cast<int>(
      round(sonia_common::FreePhysicalMemory(sonia_common::BitUnit::MB)));

  ASSERT_GT(free_space, 0);

  // The size of the file in MB
  unsigned int size = 32;
  FILE *fp = fopen("/tmp/atlas_fsinfo_test", "w");
  ftruncate(fileno(fp), size * 1024 * 1024);
  fclose(fp);

  // Asserting that new the available space has changed
  auto new_free_space = static_cast<int>(
      round(sonia_common::FreePhysicalMemory(sonia_common::BitUnit::MB)));

  // Removing the created file
  remove("/tmp/atlas_fsinfo_test");

  // Accept an error margin
  auto margin = 50;
  ASSERT_LT(free_space, new_free_space + size + margin);
  ASSERT_GT(free_space, new_free_space + size - margin);
}

TEST(FSInfo, availableSpace) {
  auto avail_space = static_cast<int>(
      round(sonia_common::AvailablePhysicalMemory(sonia_common::BitUnit::MB)));

  ASSERT_GT(avail_space, 0);

  // The size of the file in MB
  unsigned int size = 32;
  FILE *fp = fopen("/tmp/atlas_fsinfo_test", "w");
  ftruncate(fileno(fp), size * 1024 * 1024);
  fclose(fp);

  // Asserting that new the available space has changed
  auto new_avail_space = static_cast<int>(
      round(sonia_common::AvailablePhysicalMemory(sonia_common::BitUnit::MB)));

  // Removing the created file
  remove("/tmp/atlas_fsinfo_test");

  // Accept an error margin
  auto margin = 50;
  ASSERT_LT(avail_space, new_avail_space + size + margin);
  ASSERT_GT(avail_space, new_avail_space + size - margin);
}

TEST(FSInfo, totalSpace) {
  auto total_space = sonia_common::TotalPhysicalMemory();

  ASSERT_GT(total_space, 0);
}

TEST(FSInfo, percentageUsedSpace) {
  auto value = sonia_common::PercentageUsedPhysicalMemory();

  ASSERT_GT(value, 0);
  ASSERT_LT(value, 1);
}

TEST(FSInfo, percentageAvailableSpace) {
  auto value = sonia_common::PercentageAvailablePhysicalMemory();

  ASSERT_GT(value, 0);
  ASSERT_LT(value, 1);
}

TEST(FSInfo, dataAccuracy) {
  // Here we want to check that the data make sense.
  // We are going to comparate the data against each other to assert there is
  // no obvious error.

  auto total = sonia_common::TotalPhysicalMemory();
  auto avail = sonia_common::AvailablePhysicalMemory();
  auto free = sonia_common::FreePhysicalMemory();

  ASSERT_GT(total, avail);
  ASSERT_GT(total, free);
  ASSERT_GE(free, avail);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

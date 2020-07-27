/**
 * \file	timer_test.cc
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
#include <sonia_common/sys/timer.h>

using sonia_common::Timer;
using sonia_common::MilliTimer;

TEST(TimerTest, startTimer) {
  sonia_common::NanoTimer timer;

  timer.Start();
  timer.Sleep(10);
  sonia_common::Timer<>::Sleep(10);
  auto value1 = timer.Time();
  timer.Sleep(10);
  auto value2 = timer.Time();

  ASSERT_TRUE(timer.IsRunning());
  ASSERT_GT(value1, 0);
  ASSERT_GT(value2, value1);
}

TEST(TimerTest, pauseTimer) {
  sonia_common::NanoTimer timer;

  timer.Start();
  timer.Pause();
  auto value1 = timer.Time();
  auto value2 = timer.Time();

  ASSERT_FALSE(timer.IsRunning());
  ASSERT_GT(value1, 0);
  ASSERT_EQ(value1, value2);
}

TEST(TimerTest, unpauseTimer) {
  sonia_common::NanoTimer timer;

  timer.Start();
  timer.Pause();
  auto value1 = timer.Time();
  timer.Unpause();
  auto value2 = timer.Time();

  ASSERT_TRUE(timer.IsRunning());
  ASSERT_GT(value1, 0);
  ASSERT_GT(value2, value1);
}

/**
 * Do 100 trials of timing gaps between 0 and 19 milliseconds.
 * Expect accuracy within one millisecond.
 */
TEST(timer_tests, short_intervals) {
  for (int trial = 0; trial < 100; trial++)
  {
    uint32_t ms = rand() % 20;
    MilliTimer mt(ms);
    mt.Sleep(ms);
    int32_t r = mt.Remaining();

    // 1ms slush, for the cost of calling usleep.
    // Putting 4 here. For some reason, 1ms of margin is not enough...
    EXPECT_NEAR(r+1, 0, 4);
  }
}

TEST(timer_tests, overlapping_long_intervals) {
  MilliTimer* timers[10];

  // Experimentally determined. Corresponds to the extra time taken by the loops,
  // the big usleep, and the test infrastructure itself.
  const int slush_factor = 14;

  // Set up the timers to each time one second, 1ms apart.
  for (int t = 0; t < 10; t++)
  {
    timers[t] = new MilliTimer(1000);
    usleep(1000);
  }

  // Check in on them after 500ms.
  usleep(500000);
  for (int t = 0; t < 10; t++)
  {
    EXPECT_NEAR(timers[t]->Remaining(), 500 - slush_factor + t, 5);
  }

  // Check in on them again after another 500ms and free them.
  usleep(500000);
  for (int t = 0; t < 10; t++)
  {
    EXPECT_NEAR(timers[t]->Remaining(), -slush_factor + t, 5);
    delete timers[t];
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

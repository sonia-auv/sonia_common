/**
 * \file	time.h
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

#ifndef SONIA_COMMON_SYSTEM_TIMER_H_
#define SONIA_COMMON_SYSTEM_TIMER_H_

#include <sonia_common/macros.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>

namespace sonia_common {

template <class Ut_ = std::chrono::milliseconds,
          class Tp_ = std::chrono::steady_clock>
class Timer {
 public:
  //==========================================================================
  // T Y P E D E F   A N D   E N U M

  using Ptr = std::shared_ptr<Timer<Ut_, Tp_>>;

  //============================================================================
  // P U B L I C   C / D T O R S

  Timer() ATLAS_NOEXCEPT;

  explicit Timer(const uint32_t &v) ATLAS_NOEXCEPT;

  ~Timer() ATLAS_NOEXCEPT;

  //============================================================================
  // P U B L I C   S T A T I C   M E T H O D S

  /**
   * This method return the current count of the CPU.
   * The metadata can eventually be compared with another count of the CPU.
   *
   * \return the current number of count from the CPU.
   */
  static int64_t Now() ATLAS_NOEXCEPT;

  /**
   * Get the current timespec from a value in milliseconds.
   */
  static timespec TimeSpecFromMs(const uint32_t &v) ATLAS_NOEXCEPT;

  /**
   * Make a pause on the current calling thread.
   *
   * \param sleeping_time The time to sleep the current thread with the unit Ut_
   */
  static void Sleep(int64_t sleeping_time) ATLAS_NOEXCEPT;

  /**
   * Return the current date and time with the current format.
   *
   * The return format is YYYY-MM-DD.HH:mm:ss
   *
   * \return The current date and time as a string.
   */
  static std::string CurrentDate() ATLAS_NOEXCEPT;

  //============================================================================
  // P U B L I C   M E T H O D S

  /**
   * Start the timer
   *
   * If the timer was running, this will reset the timer before restarting it.
   */
  void Start() ATLAS_NOEXCEPT;

  /**
   * Pause the timer if it is not running.
   * This will throw a std::logic_error exception if the timer is not running.
   */
  void Pause();

  /**
   * Unpause a paused timer.
   * This will throw a std::logic_error exception if the timer is running.
   */
  void Unpause();

  /**
   * Reset the timer by setting both the start and the pause time to now.
   */
  void Reset() ATLAS_NOEXCEPT;

  /**
   * Return the remaining time if one was provided on the construction
   * of the timer. Throw an exception if not.
   */
  int64_t Remaining() ATLAS_NOEXCEPT;

  /**
   * \return Either if the timer is running or being paused.
   */
  bool IsRunning() ATLAS_NOEXCEPT;

  /**
   * Get the difference between now and the starting time with the give unit.
   * The unit must be compatible with std::chrono units
   * -- e.g. std::chrono::seconds.
   *
   * \tparam Tp_ The unit the result will be output with.
   * \return the elapsed time from the starting point to now.
   */
  template <class Yp_ = Ut_>
  double Time() const ATLAS_NOEXCEPT;

  /**
   * Wrapper to the running_time function that return the elapsed time in
   * nanoseconds.
   *
   * \return The elapsed time in nanoseconds.
   */
  int64_t NanoSeconds() const ATLAS_NOEXCEPT;

  /**
   * Wrapper to the running_time function that return the elapsed time in
   * microseconds.
   *
   * \return The elapsed time in microseconds.
   */
  int64_t MicroSeconds() const ATLAS_NOEXCEPT;

  /**
   * Wrapper to the running_time function that return the elapsed time in
   * milliseconds.
   *
   * \return The elapsed time in milliseconds.
   */
  int64_t MilliSeconds() const ATLAS_NOEXCEPT;

  /**
   * Wrapper to the running_time function that return the elapsed time in
   * seconds.
   *
   * \return The elapsed time in seconds.
   */
  int64_t Seconds() const ATLAS_NOEXCEPT;

  /**
   * Wrapper to the running_time function that return the elapsed time in
   * minutes.
   *
   * \return The elapsed time in minutes.
   */
  int64_t Minutes() const ATLAS_NOEXCEPT;

  /**
   * Wrapper to the running_time function that return the elapsed time in
   * hours.
   *
   * \return The elapsed time in hours.
   */
  int64_t Hours() const ATLAS_NOEXCEPT;

 private:
  //============================================================================
  // P R I V A T E   M E T H O D S

  static timespec TimeSpecNow() ATLAS_NOEXCEPT;

  //============================================================================
  // P R I V A T E   M E M B E R S

  bool is_running_ = {false};

  timespec expiry_;

  typename Tp_::time_point start_time_ = {};

  typename Tp_::time_point pause_time_ = {};

  mutable std::mutex member_guard_ = {};
};

using SecTimer = Timer<std::chrono::seconds, std::chrono::steady_clock>;
using MilliTimer = Timer<std::chrono::milliseconds, std::chrono::steady_clock>;
using MicroTimer = Timer<std::chrono::microseconds, std::chrono::steady_clock>;
using NanoTimer = Timer<std::chrono::nanoseconds, std::chrono::steady_clock>;

}  // namespace sonia_common

#include <sonia_common/sys/timer_inl.h>

#endif  // SONIA_COMMON_SYSTEM_TIMER_H_

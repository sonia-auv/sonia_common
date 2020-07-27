/**
 * \file	timer_inl.cc
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
#error This file may only be included from timer.h
#endif

#include <sonia_common/macros.h>
#include <math.h>
#include <thread>
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

namespace sonia_common {

//==============================================================================
// C / D T O R   S E C T I O N

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE Timer<Up_, Tp_>::Timer() ATLAS_NOEXCEPT {}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE Timer<Up_, Tp_>::Timer(const uint32_t &v) ATLAS_NOEXCEPT
    : expiry_(TimeSpecNow()) {
  int64_t tv_nsec = expiry_.tv_nsec + (v * 1e6);
  if (tv_nsec >= 1e9) {
    int64_t sec_diff = tv_nsec / static_cast<int>(1e9);
    expiry_.tv_nsec = tv_nsec - static_cast<int>(1e9 * sec_diff);
    expiry_.tv_sec += sec_diff;
  } else {
    expiry_.tv_nsec = tv_nsec;
  }
}
//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE Timer<Up_, Tp_>::~Timer() ATLAS_NOEXCEPT {}

//==============================================================================
// M E T H O D S   S E C T I O N

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE void Timer<Up_, Tp_>::Start() ATLAS_NOEXCEPT {
  Reset();
  std::lock_guard<std::mutex> guard(member_guard_);
  is_running_ = true;
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE void Timer<Up_, Tp_>::Reset() ATLAS_NOEXCEPT {
  std::lock_guard<std::mutex> guard(member_guard_);
  start_time_ = Tp_::now();
  pause_time_ = start_time_;
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE void Timer<Up_, Tp_>::Pause() {
  std::lock_guard<std::mutex> guard(member_guard_);
  if (!is_running_) {
    guard.~lock_guard();
    throw std::logic_error("The timer is not running");
  }
  pause_time_ = Tp_::now();
  is_running_ = false;
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE void Timer<Up_, Tp_>::Unpause() {
  std::lock_guard<std::mutex> guard(member_guard_);
  if (is_running_) {
    guard.~lock_guard();
    throw std::logic_error("The timer is running");
  }
  start_time_ += Tp_::now() - pause_time_;
  is_running_ = true;
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE int64_t Timer<Up_, Tp_>::Remaining() ATLAS_NOEXCEPT {
  timespec now(TimeSpecNow());
  int64_t millis = (expiry_.tv_sec - now.tv_sec) * 1e3;
  millis += (expiry_.tv_nsec - now.tv_nsec) / 1e6;
  return millis;
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE bool Timer<Up_, Tp_>::IsRunning() ATLAS_NOEXCEPT {
  std::lock_guard<std::mutex> guard(member_guard_);
  return is_running_;
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
template <typename Yp_>
ATLAS_ALWAYS_INLINE double Timer<Up_, Tp_>::Time() const ATLAS_NOEXCEPT {
  std::lock_guard<std::mutex> guard(member_guard_);
  auto time = Tp_::now() - start_time_;
  if (!is_running_) {
    time = pause_time_ - start_time_;
  }
  auto period = static_cast<double>(Yp_::period::num) /
                static_cast<double>(Yp_::period::den);
  return static_cast<double>(std::chrono::duration_cast<Yp_>(time).count() *
                             period);
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE int64_t Timer<Up_, Tp_>::Now() ATLAS_NOEXCEPT {
  return std::chrono::duration_cast<Up_>(Tp_::now().time_since_epoch()).count();
}
//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE timespec Timer<Up_, Tp_>::TimeSpecFromMs(const uint32_t &v)
    ATLAS_NOEXCEPT {
  timespec time;
  time.tv_sec = v / 1e3;
  time.tv_nsec = (v - (time.tv_sec * 1e3)) * 1e6;
  return time;
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE std::string Timer<Up_, Tp_>::CurrentDate() ATLAS_NOEXCEPT {
  time_t now = time(0);
  struct tm tstruct;
  char *buf = nullptr;
  tstruct = *localtime(&now);
  // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
  // for more information about date/time format
  std::strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
  return buf;
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE void Timer<Up_, Tp_>::Sleep(int64_t sleeping_time)
    ATLAS_NOEXCEPT {
  std::this_thread::sleep_for(Up_(sleeping_time));
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE int64_t
Timer<Up_, Tp_>::NanoSeconds() const ATLAS_NOEXCEPT {
  return static_cast<int64_t>(Time<std::chrono::nanoseconds>() * 1000000000);
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE int64_t
Timer<Up_, Tp_>::MicroSeconds() const ATLAS_NOEXCEPT {
  return static_cast<int64_t>(Time<std::chrono::microseconds>() * 1000000);
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE int64_t
Timer<Up_, Tp_>::MilliSeconds() const ATLAS_NOEXCEPT {
  return static_cast<int64_t>(Time<std::chrono::milliseconds>() * 1000);
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE int64_t Timer<Up_, Tp_>::Seconds() const ATLAS_NOEXCEPT {
  return static_cast<int64_t>(Time<std::chrono::seconds>());
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE int64_t Timer<Up_, Tp_>::Minutes() const ATLAS_NOEXCEPT {
  return static_cast<int64_t>(Time<std::chrono::minutes>() / 60);
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE int64_t Timer<Up_, Tp_>::Hours() const ATLAS_NOEXCEPT {
  return static_cast<int64_t>(Time<std::chrono::hours>() / 3600);
}

//------------------------------------------------------------------------------
//
template <class Up_, class Tp_>
ATLAS_ALWAYS_INLINE timespec Timer<Up_, Tp_>::TimeSpecNow() ATLAS_NOEXCEPT {
  timespec time;
#ifdef __MACH__  // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  time.tv_sec = mts.tv_sec;
  time.tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, &time);
#endif
  return time;
}

}  // namespace sonia_common

/**
 * \file	runnable.h
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

#ifndef SONIA_COMMON_PATTERN_RUNNABLE_H_
#define SONIA_COMMON_PATTERN_RUNNABLE_H_

#include <sonia_common/macros.h>
#include <atomic>
#include <memory>
#include <thread>

namespace sonia_common {

/**
 * Runnable is  a simple wrapper around C++11 thread.
 * It is design to allow a class that aims to provide a parrallel task to do
 * in without any overhead.
 *
 * A Runnable instance can be started and stoped.
 * The user of this class must consider checking the state of the thread with
 * running when implementing its run method (or any other method that will try
 * to access the thread). The running() method as been provided at this effect.
 */
class Runnable {
 public:
  //==========================================================================
  // T Y P E D E F   A N D   E N U M

  using Ptr = std::shared_ptr<Runnable>;

  //============================================================================
  // P U B L I C   C / D T O R S

  Runnable() ATLAS_NOEXCEPT;

  virtual ~Runnable() ATLAS_NOEXCEPT;

  /**
   * Makes no sense to copy a thread, delete the copy ctor instead.
   */
  explicit Runnable(const Runnable &) = delete;

  explicit Runnable(Runnable &&) = delete;

  //============================================================================
  // P U B L I C   O P E R A T O R S

  /**
   * Makes no sense to copy a thread, delete the copy operator instead.
   */
  Runnable &operator=(const Runnable &) = delete;

  Runnable &operator=(Runnable &&) = delete;

  //============================================================================
  // P U B L I C  M E T H O D S

  /**
   * Start the parrallel task of this Runnable instance.
   *
   * This will create a new thread with the Runnable.run() method.
   */
  void Start();

  /**
   * Stop the parallel task.
   *
   * This will state the stop_ data member to true and join the thread member.
   * Thus, a derived class that implement the Runnable interface and provid
   * a looping parallel task must check for the stop_ member state, otherwise,
   * the stop() call will be blocking forever.
   */
  void Stop() ATLAS_NOEXCEPT_(false);

  /**
   * Return either if the derived Runnable is currently running or not.
   *
   * This is a simple wrapper around the thread_ is joinable method.
   *
   * \return The running state of the thread member.
   */
  bool IsRunning() const ATLAS_NOEXCEPT;

 protected:
  //============================================================================
  // P R O T E C T E D   M E T H O D S

  /**
   * This is the actual method that must be implemented by derived Runnable
   * classes.
   * The run method will be called when the Runnable object is started (using
   * start()).
   * If the Runnable class must be running during the whole lifetime of the
   * thread member, implement your loop by checking the state of the thread (
   * calling running()). This way, no unexpected behavior will appear when
   * the thread is interrupted.
   */
  virtual void Run() = 0;

  /**
   * Indicates either if this Runnable instance must stop its processing or not.
   *
   * This is to provide derived class a way to know when the stop() method has
   * been called into the run method. If the run() method is a looping method,
   * check for the result of the must_stop() method.
   *
   * \return Either if the parallel task must stop or not.
   */
  bool MustStop() const ATLAS_NOEXCEPT;

 private:
  //============================================================================
  // P R I V A T E   M E M B E R S

  std::unique_ptr<std::thread> thread_;

  std::atomic<bool> stop_;
};

}  // namespace sonia_common

#include <sonia_common/pattern/runnable_inl.h>

#endif  // SONIA_COMMON_PATTERN_RUNNABLE_H_

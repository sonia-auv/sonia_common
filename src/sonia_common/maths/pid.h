/**
 * \file	pid.h
 * \author	Thibaut Mattio <thibaut.mattio@gmail.com>
 * \date	24/05/2016
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

#ifndef SONIA_COMMON_MATHS_PID_H_
#define SONIA_COMMON_MATHS_PID_H_

#include <sonia_common/macros.h>
#include <math.h>
#include <eigen3/Eigen/Eigen>

namespace sonia_common {

/// PID is a class that encapsulate the logic of a PID controller.
/// You must set the gains and the limits of the values, and the method Refresh
/// does the calculations.
/// This implementation is based on: https://github.com/gregorygusberti/PID
/// A good ressource is the Paper PID without a PhD that you can find here:
/// http://www.wescottdesign.com/articles/pid/pidWithoutAPhd.pdf
class PID {
 public:
  struct Weights {
    double p;
    double i;
    double d;
  };

  //============================================================================
  // P U B L I C   C / D T O R S

  /// Public constructor
  PID();

  /// Set the proportional term. Use it AFTER setRefreshInterval or
  /// setRefreshRate
  void SetKp(const double &Kp);

  /// Set the integral term. Use it AFTER setRefreshInterval or setRefreshRate
  void SetKi(const double &Ki);

  /// Set the derivative term. Use it AFTER setRefreshInterval or setRefreshRate
  void SetKd(const double &Kd);

  /// Set the P, I, D terms respectively. Use it AFTER setRefreshInterval or
  /// setRefreshRate
  void SetWeights(const double &Kp, const double &Ki, const double &Kd);

  /// Set the refresh interval of the controller in seconds.
  /// If you don't know the interval you can set the frequency of update using
  /// setRefreshRate
  void SetRefreshInterval(const double &refresh_interval);

  /// Set the refresh frequency of the controller in hertz.
  /// If you don't know the refresh frequency you can set the refresh interval
  /// using SetRefreshInterval
  void SetRefreshRate(const double &refresh_rate);

  /// Set the minimun error for computation of the PID loop. The default is 0.0
  /// It can be set to prevent integral windup or updates with minimal errors
  void SetErrorThreshold(const double &error_threshold);

  /// Set the lower limit of the output. Output will be clamped to this value.
  /// If clamped, the integral will not be computed to prevent integral windup.
  /// To set the upper limit use setOutputUpperLimit
  /// YOU NEED TO SET IT!
  void SetOutputLowerLimit(const double &output_lower_limit);

  /// Set the upper limit of the output. Output will be clamped to this value.
  /// If clamped, the integral will not be computed to prevent integral windup.
  /// To set the lower limit use setOutputLowerLimit
  /// YOU NEED TO SET IT!
  void SetOutputUpperLimit(const double &output_upper_limit);

  /// Set the desired point of the output.
  /// This is the output the PID loop will try to reach.
  /// error will be computed subtracting the feedback input from this value
  void SetDesiredPoint(const double &desired_point);

  /// The refresh function that make all the PID computations.
  /// Call it at specific rate, setted by setRefreshRate or setRefreshInterval
  /// (read descriptions).
  /// (use a timer interrupt if used on a embedded microcontroller)
  /// The feedback_input is the value from the feedback.
  /// The returned value is the output value of the filter.
  double Refresh(const double &feedback_input);

 private:
  Weights k_;
  double last_error_;
  double last_output_;
  double set_point_;
  double integral_;
  double interval_;
  double error_threshold_;
  double output_upper_limit_;
  double output_lower_limit_;
  double error_;
};

}  // namespace sonia_common

#include <sonia_common/maths/pid_inl.h>

#endif  // SONIA_COMMON_MATHS_PID_H_

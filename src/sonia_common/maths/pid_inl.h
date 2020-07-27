/**
 * \file	pid_inl.h
 * \author	Thibaut Mattio <thibaut.mattio@gmail.com>
 * \date	25/05/2016
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
#error This file may only be included pid.h
#endif  // SONIA_COMMON_MATHS_PID_H_

namespace sonia_common {

//------------------------------------------------------------------------------
//
ATLAS_INLINE PID::PID()
    : last_error_(0.),
      last_output_(0.),
      integral_(0.),
      interval_(1.),
      error_threshold_(0.) {}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void PID::SetKp(const double &Kp) { k_.p = Kp; }

//------------------------------------------------------------------------------
//
ATLAS_INLINE void PID::SetKi(const double &Ki) { k_.i = Ki * interval_; }

//------------------------------------------------------------------------------
//
ATLAS_INLINE void PID::SetKd(const double &Kd) { k_.d = Kd / interval_; }

//------------------------------------------------------------------------------
//
ATLAS_INLINE void PID::SetWeights(const double &Kp, const double &Ki,
                                  const double &Kd) {
  SetKp(Kp);
  SetKi(Ki);
  SetKd(Kd);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void PID::SetRefreshInterval(const double &refresh_interval) {
  interval_ = refresh_interval;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void PID::SetRefreshRate(const double &refresh_rate) {
  interval_ = 1.f / refresh_rate;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void PID::SetErrorThreshold(const double &error_threshold) {
  error_threshold_ = error_threshold;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void PID::SetOutputLowerLimit(const double &output_lower_limit) {
  output_lower_limit_ = output_lower_limit;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void PID::SetOutputUpperLimit(const double &output_upper_limit) {
  output_upper_limit_ = output_upper_limit;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void PID::SetDesiredPoint(const double &desired_point) {
  set_point_ = desired_point;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE double PID::Refresh(const double &feedback_input) {
  error_ = set_point_ - feedback_input;
  if (error_ >= fabs(error_threshold_) && error_ != 0) {
    last_output_ =
        k_.p * error_ + k_.i * integral_ + k_.d * (error_ - last_error_);
    if (last_output_ > output_upper_limit_) {
      last_output_ = output_upper_limit_;
      if (integral_ / error_ < 0.f) {
        integral_ += (error_ + last_error_) / 2.f;
        last_error_ = error_;
      }
      return output_upper_limit_;
    }
    if (last_output_ < output_lower_limit_) {
      if (integral_ / error_ < 0.f) {
        integral_ += (error_ + last_error_) / 2.f;
        last_error_ = error_;
      }
      last_output_ = output_lower_limit_;
      return output_lower_limit_;
    }
    integral_ += (error_ + last_error_) / 2.f;
    last_error_ = error_;
  }
  return last_output_;
}

}  // namespace sonia_common

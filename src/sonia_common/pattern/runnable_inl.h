/**
 * \file	runnable_inl.h
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
#error This file may only be included from runnable.h
#endif

namespace sonia_common {

//==============================================================================
// C / D T O R   S E C T I O N

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE Runnable::Runnable() ATLAS_NOEXCEPT : thread_(nullptr),
                                                          stop_(false) {}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE Runnable::~Runnable() ATLAS_NOEXCEPT {
  if (IsRunning()) {
    Stop();
  }
}

//==============================================================================
// M E T H O D S   S E C T I O N

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE void Runnable::Start() {
  if (thread_ == nullptr) {
    thread_ =
        std::unique_ptr<std::thread>(new std::thread(&Runnable::Run, this));
  } else {
    throw std::logic_error("The thread must be stoped before it is started.");
  }
}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE void Runnable::Stop() ATLAS_NOEXCEPT {
  if (IsRunning()) {
    stop_ = true;
    thread_->join();
    thread_ = nullptr;
  } else {
    throw std::logic_error("The thread is not running.");
  }
}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE bool Runnable::IsRunning() const ATLAS_NOEXCEPT {
  return thread_ != nullptr && thread_->joinable() && !MustStop();
}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE bool Runnable::MustStop() const ATLAS_NOEXCEPT {
  return stop_;
}

}  // namespace sonia_common

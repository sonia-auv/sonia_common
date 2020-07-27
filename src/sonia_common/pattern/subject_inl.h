/**
 * \file	observer_inl.h
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

#ifndef SONIA_COMMON_PATTERN_SUBJECT_H_
#error This file may only be included from subject.h
#endif

#include <assert.h>
#include <sonia_common/pattern/observer.h>
#include <algorithm>

namespace sonia_common {

//==============================================================================
// C / D T O R S   S E C T I O N

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE Subject<Args_...>::Subject() ATLAS_NOEXCEPT
    : observers_(),
      observers_mutex_() {}

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE Subject<Args_...>::Subject(const Subject<Args_...> &rhs)
    ATLAS_NOEXCEPT : observers_(),
                     observers_mutex_() {
  for (auto &observer : rhs.observers_) {
    observer->Observe(*this);
  }
}
//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE Subject<Args_...>::~Subject() ATLAS_NOEXCEPT {
  for (const auto &observer : observers_) {
    observer->OnSubjectDisconnected(*this);
  }
}

//==============================================================================
// O P E R A T O R S   S E C T I O N

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE void Subject<Args_...>::operator=(
    const Subject<Args_...> &rhs) ATLAS_NOEXCEPT {
  DetachAll();
  for (auto &observer : rhs.observers_) {
    Attach(*observer);
  }
}

//==============================================================================
// M E T H O D S   S E C T I O N

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE void Subject<Args_...>::Attach(
    Observer<Args_...> &observer) {
  std::unique_lock<std::mutex> locker(observers_mutex_);

  auto it = std::find(observers_.begin(), observers_.end(), &observer);
  if (it != observers_.end()) {
    throw std::invalid_argument("The element is already in the container.");
  } else {
    observers_.push_back(&observer);
  }
  observer.OnSubjectConnected(*this);
}

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE void Subject<Args_...>::Detach(
    Observer<Args_...> &observer) {
  std::unique_lock<std::mutex> locker(observers_mutex_);
  auto it = std::find(observers_.begin(), observers_.end(), &observer);
  if (it == observers_.end()) {
    throw std::invalid_argument("The element is not in the container.");
  } else {
    observers_.erase(it);
  }
  observer.OnSubjectDisconnected(*this);
}

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE void Subject<Args_...>::DetachNoCallback(
    Observer<Args_...> &observer) {
  std::unique_lock<std::mutex> locker(observers_mutex_);
  auto it = std::find(observers_.begin(), observers_.end(), &observer);
  if (it == observers_.end()) {
    throw std::invalid_argument("The element is not in the container.");
  } else {
    observers_.erase(it);
  }
}

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE void Subject<Args_...>::DetachAll() ATLAS_NOEXCEPT {
  for (const auto &observer : observers_) {
    Detach(*observer);
  }
}

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE void Subject<Args_...>::Notify(Args_... args)
    ATLAS_NOEXCEPT {
  std::unique_lock<std::mutex> locker(observers_mutex_);
  for (const auto &observer : observers_) {
    observer->OnSubjectNotify(*this, args...);
  }
}

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE size_t
Subject<Args_...>::ObserverCount() const ATLAS_NOEXCEPT {
  std::unique_lock<std::mutex> locker(observers_mutex_);
  return observers_.size();
}

}  // namespace sonia_common

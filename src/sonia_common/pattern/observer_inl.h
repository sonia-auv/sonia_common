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

#ifndef SONIA_COMMON_PATTERN_OBSERVER_H_
#error This file may only be included from observer.h
#endif

#include <algorithm>
#include <cassert>

namespace sonia_common {

//==============================================================================
// C / D T O R S   S E C T I O N

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE Observer<Args_...>::Observer() ATLAS_NOEXCEPT
    : subjects_(),
      subjects_mutex_() {}

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE Observer<Args_...>::Observer(const Observer<Args_...> &rhs)
    ATLAS_NOEXCEPT : subjects_(),
                     subjects_mutex_() {
  for (auto &subject : rhs.subjects_) {
    subject->Attach(*this);
  }
}

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE Observer<Args_...>::Observer(Subject<Args_...> &subject)
    ATLAS_NOEXCEPT : subjects_(),
                     subjects_mutex_() {
  subject.Attach(*this);
}

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE Observer<Args_...>::~Observer() ATLAS_NOEXCEPT {
  for (const auto &subject : subjects_) {
    subject->DetachNoCallback(*this);
  }
}

//==============================================================================
// O P E R A T O R S   S E C T I O N

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE void Observer<Args_...>::operator=(
    const Observer<Args_...> &rhs) ATLAS_NOEXCEPT {
  DetachFromAllSubject();
  for (auto &subject : rhs.subjects_) {
    subject->Attach(*this);
  }
}

//==============================================================================
// M E T H O D S   S E C T I O N

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE void Observer<Args_...>::DetachFromAllSubject()
    ATLAS_NOEXCEPT {
  // Do not lock the mutex here because it will be in OnSubjectDisconnected().
  for (const auto &subject : subjects_) {
    subject->Detach(*this);
  }
  subjects_.clear();
}

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE void Observer<Args_...>::OnSubjectConnected(
    Subject<Args_...> &subject) {
  std::unique_lock<std::mutex> locker(subjects_mutex_);
  auto it = std::find(subjects_.begin(), subjects_.end(), &subject);
  if (it != subjects_.end()) {
    throw std::invalid_argument("The element is already in the container.");
  } else {
    subjects_.push_back(&subject);
  }
}

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE void Observer<Args_...>::OnSubjectDisconnected(
    Subject<Args_...> &subject) {
  std::unique_lock<std::mutex> locker(subjects_mutex_);
  auto it = std::find(subjects_.begin(), subjects_.end(), &subject);
  if (it == subjects_.end()) {
    throw std::invalid_argument("The element is not in the container.");
  } else {
    subjects_.erase(it);
  }
}

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE bool Observer<Args_...>::IsAttached(
    const Subject<Args_...> &subject) const ATLAS_NOEXCEPT {
  std::unique_lock<std::mutex> locker(subjects_mutex_);
  return std::find(subjects_.begin(), subjects_.end(), &subject) !=
         subjects_.end();
}

//------------------------------------------------------------------------------
//
template <typename... Args_>
ATLAS_ALWAYS_INLINE void Observer<Args_...>::Observe(
    Subject<Args_...> &subject) {
  subject.Attach(*this);
}

}  // namespace sonia_common

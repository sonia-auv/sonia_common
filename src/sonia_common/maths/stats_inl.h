/**
 * \file	stats_inl.h
 * \author	Thibaut Mattio <thibaut.mattio@gmail.com>
 * \author  Antoine Dozois <dozois.a@gmail.com>
 * \date	17/08/2015
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

#ifndef SONIA_COMMON_MATHS_STATS_H_
#error This file may only be included from stats.h
#endif

#include <math.h>
#include <algorithm>

namespace sonia_common {

namespace details {

// Allow us to check if the type we received is actually iterable.
// If the data set must be iterable, we can static assert it with this
// type traits.
// Fore more informations:
// http://stackoverflow.com/questions/13830158/check-if-a-variable-is-iterable

//------------------------------------------------------------------------------
//
template <typename Tp_>
auto IsIterableImpl(int)
    -> decltype(std::begin(std::declval<Tp_ &>()) !=
                    std::end(std::declval<Tp_ &>()),
                ++std::declval<decltype(std::begin(std::declval<Tp_ &>())) &>(),
                *begin(std::declval<Tp_ &>()), std::true_type{});

template <typename Tp_>
std::false_type IsIterableImpl(...);

template <typename Tp_>
using IsIterable = decltype(IsIterableImpl<Tp_>(0));

}  // namespace details

//------------------------------------------------------------------------------
//
template <typename Tp_, typename Up_>
ATLAS_ALWAYS_INLINE double Euclidean(const Tp_ &v1, const Up_ &v2) {
  static_assert(details::IsIterable<Tp_>::value,
                "The data set must be iterable");
  static_assert(details::IsIterable<Up_>::value,
                "The data set must be iterable");
  if (v1.size() != v2.size()) {
    throw std::invalid_argument("The lengh of the data set is not the same");
  }

  typename Tp_::value_type diff = {v1[0] - v2[0]};
  typename Tp_::value_type s = {diff * diff};

  for (uint64_t i = 1; i < v1.size(); ++i) {
    diff = v1[i] - v2[i];
    s += diff * diff;
  }
  return sqrt(static_cast<double>(s));
}

//------------------------------------------------------------------------------
//
template <typename Tp_, typename Up_>
ATLAS_ALWAYS_INLINE double Jaccard(const Tp_ &v1, const Up_ &v2) {
  static_assert(details::IsIterable<Tp_>::value,
                "The data set must be iterable");
  if (v1.size() != v2.size()) {
    throw std::invalid_argument("The lengh of the data set is not the same");
  }

  uint64_t eq = {0};
  uint64_t nq = {0};

  for (uint64_t i = 0; i < v1.size(); ++i) {
    if (v1[i] == v2[i]) {
      if (v1[i] != 0) {
        ++eq;
      } else {
        ++nq;
      }
    }
  }
  return static_cast<double>(eq) / static_cast<double>(v1.size() - (nq + eq));
}

//------------------------------------------------------------------------------
//
template <typename Tp_>
ATLAS_ALWAYS_INLINE double Mean(const Tp_ &v) ATLAS_NOEXCEPT {
  static_assert(details::IsIterable<Tp_>::value,
                "The data set must be iterable");
  typename Tp_::value_type s = {0};
  for (const auto &e : v) {
    s += e;
  }
  return static_cast<double>(s) / static_cast<double>(v.size());
}

//------------------------------------------------------------------------------
//
template <typename Tp_>
ATLAS_ALWAYS_INLINE typename Tp_::value_type Median(const Tp_ &v)
    ATLAS_NOEXCEPT {
  static_assert(details::IsIterable<Tp_>::value,
                "The data set must be iterable");
  Tp_ sorted_data = {v};
  std::sort(sorted_data.begin(), sorted_data.end());
  return sorted_data.at(ceil(sorted_data.size() / 2));
}

//------------------------------------------------------------------------------
//
template <typename Tp_>
ATLAS_ALWAYS_INLINE double GeometricMean(const Tp_ &v) ATLAS_NOEXCEPT {
  static_assert(details::IsIterable<Tp_>::value,
                "The data set must be iterable");
  double sum = 1.f;

  for (const auto &e : v) {
    sum *= e;
  }
  return pow(sum, 1.0 / static_cast<double>(v.size()));
}

//------------------------------------------------------------------------------
//
template <typename Tp_>
ATLAS_ALWAYS_INLINE double HarmonicMean(const Tp_ &v) {
  static_assert(details::IsIterable<Tp_>::value,
                "The data set must be iterable");
  double harmonic = 0.;
  for (const auto &e : v) {
    harmonic += 1. / static_cast<double>(e);
  }

  if (harmonic == 0) {
    throw std::logic_error("The harmonic equal zero! Can't return result");
  }

  return static_cast<double>(v.size()) / harmonic;
}

//------------------------------------------------------------------------------
//
template <typename Tp_>
ATLAS_ALWAYS_INLINE typename Tp_::value_type Min(const Tp_ &v) ATLAS_NOEXCEPT {
  static_assert(details::IsIterable<Tp_>::value,
                "The data set must be iterable");
  return *std::min_element(v.cbegin(), v.cend());
}

//------------------------------------------------------------------------------
//
template <typename Tp_>
ATLAS_ALWAYS_INLINE typename Tp_::value_type Max(const Tp_ &v) ATLAS_NOEXCEPT {
  static_assert(details::IsIterable<Tp_>::value,
                "The data set must be iterable");
  return *std::max_element(v.cbegin(), v.cend());
}

//------------------------------------------------------------------------------
//
template <typename Tp_, typename Up_>
ATLAS_ALWAYS_INLINE double Covariance(const Tp_ &v1, const Up_ &v2) {
  static_assert(details::IsIterable<Tp_>::value,
                "The data set must be iterable");
  static_assert(details::IsIterable<Up_>::value,
                "The data set must be iterable");
  if (v1.size() != v2.size()) {
    throw std::invalid_argument("The lengh of the data set is not the same");
  }

  double m1 = Mean(v1);
  double m2 = Mean(v2);
  double s =
      (static_cast<double>(v1[0]) - m1) * (static_cast<double>(v2[0]) - m2);

  for (uint64_t i = 1; i < v1.size(); ++i) {
    s += (static_cast<double>(v1[i]) - m1) * (static_cast<double>(v2[i]) - m2);
  }
  return s / static_cast<double>(v1.size() - 1);
}

//------------------------------------------------------------------------------
//
template <typename Tp_>
ATLAS_ALWAYS_INLINE double StdDeviation(const Tp_ &v) ATLAS_NOEXCEPT {
  static_assert(details::IsIterable<Tp_>::value,
                "The data set must be iterable");
  return sqrt(Covariance(v, v));
}

//------------------------------------------------------------------------------
//
template <typename Tp_, typename Up_>
ATLAS_ALWAYS_INLINE double Pearson(const Tp_ &v1, const Up_ &v2) {
  static_assert(details::IsIterable<Tp_>::value,
                "The data set must be iterable");
  static_assert(details::IsIterable<Up_>::value,
                "The data set must be iterable");
  double std_dev1 = StdDeviation(v1);
  double std_dev2 = StdDeviation(v2);

  if (std_dev1 * std_dev2 == 0) {
    throw std::invalid_argument("The standart deviation of these set is null.");
  }
  return Covariance(v1, v2) / (std_dev1 * std_dev2);
}

}  // namespace sonia_common

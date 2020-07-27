/**
 * \file	macros.h
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

#ifndef SONIA_COMMON_MACROS_H_
#define SONIA_COMMON_MACROS_H_

#include <iostream>

#define ATLAS_THROW(exceptionClass, message)                         \
  {                                                                  \
    std::stringstream ss;                                            \
    ss << message << " at " << __FILE__ << ":" << __LINE__ << " in " \
       << __FUNCTION__;                                              \
    throw exceptionClass(ss.str().c_str());                          \
  }

// Defining exception macros
#if (__cplusplus >= 201103L)
#define ATLAS_NOEXCEPT noexcept
#define ATLAS_NOEXCEPT_(x) noexcept(x)
#define ATLAS_NOEXCEPT_OR_FALSE(x) noexcept(x)
#else
#define ATLAS_NOEXCEPT throw()
#define ATLAS_NOEXCEPT_(x)
#define ATLAS_NOEXCEPT_OR_FALSE(x) false
#endif

// Defining inline macros
#ifndef ATLAS_ALWAYS_INLINE
#define ATLAS_ALWAYS_INLINE \
  __attribute__((__visibility__("default"), __always_inline__)) inline
#endif

#ifndef ATLAS_INLINE
#define ATLAS_INLINE inline
#endif

// Defining OS variables
#if defined(_WIN32)
#define OS_WINDOWS 1
#elif defined(__APPLE__)
#define OS_DARWIN 1
#elif defined(__linux__)
#define OS_LINUX 1
#endif

#endif  // SONIA_COMMON_MACROS_H_

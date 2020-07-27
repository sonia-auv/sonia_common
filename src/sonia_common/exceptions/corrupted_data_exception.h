/**
 * \file	serial_exception.h
 * \author	Thibaut Mattio <thibaut.mattio@gmail.com>
 * \date	16/11/2015
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

#ifndef SONIA_COMMON_EXCEPTIONS_CORRUPTED_DATA_EXCEPTION_H_
#define SONIA_COMMON_EXCEPTIONS_CORRUPTED_DATA_EXCEPTION_H_

#include <sonia_common/macros.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

namespace sonia_common {

/**
 * Class for handling corrupted data that may be received from a device.
 * If the data cannot be parsed, an exception of this type should be thrown.
 */
class CorruptedDataException : public std::exception {
 public:
  //==========================================================================
  // T Y P E D E F   A N D   E N U M

  using Ptr = std::shared_ptr<CorruptedDataException>;

  //============================================================================
  // P U B L I C   C / D T O R S

  explicit CorruptedDataException(const char *description) ATLAS_NOEXCEPT {
    std::stringstream ss;
    ss << "CorruptedDataException " << description << " failed.";
    e_what_ = ss.str();
  }

  CorruptedDataException(const CorruptedDataException &rhs) ATLAS_NOEXCEPT
      : e_what_(rhs.e_what_) {}

  virtual ~CorruptedDataException() ATLAS_NOEXCEPT {}

  //============================================================================
  // P U B L I C   O P E R A T O R S

  CorruptedDataException &operator=(const CorruptedDataException &) = delete;

  //============================================================================
  // P U B L I C   M E T H O D S

  const char *what() const ATLAS_NOEXCEPT override { return e_what_.c_str(); }

 private:
  //============================================================================
  // P R I V A T E   M E M B E R S

  std::string e_what_;
};

}  // namespace sonia_common

#endif  // SONIA_COMMON_EXCEPTIONS_CORRUPTED_DATA_EXCEPTION_H_

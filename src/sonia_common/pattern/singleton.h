/**
 * \file	singleton.h
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

#ifndef SONIA_COMMON_PATTERN_SINGLETON_H_
#define SONIA_COMMON_PATTERN_SINGLETON_H_

#include <sonia_common/macros.h>
#include <memory>

namespace sonia_common {

/**
 * This class is an implementation of the GOF pattern Singleton.
 *
 * This aims to provide a class to extend in order to have a unique object
 * through the whole runtime. You can extends this class to have the behavior
 * applied to your own class.
 *
 * Sample usage:
 * class Foo: public Singleton<Foo>
 * {
 *  private:
 *    explicit Foo() {};
 *    ~Foo() {};
 *
 *    // Frienship so Singleton<Foo> can access the constructor and destructor.
 *    friend class Singleton<Foo>;
 *  };
 */
template <class Tp_>
class Singleton {
 public:
  //==========================================================================
  // T Y P E D E F   A N D   E N U M

  using Ptr = std::shared_ptr<Singleton<Tp_>>;

  //============================================================================
  // P U B L I C   C / D T O R S

  /**
   * Deleting the copy constructor so we are sure we cannot create another
   * instance of the singleton.
   */
  explicit Singleton(const Singleton &) ATLAS_NOEXCEPT = delete;

  virtual ~Singleton() ATLAS_NOEXCEPT = default;

  //============================================================================
  // P U B L I C   O P E R A T O R S

  /**
   * Do not implement the assignement operator.
   */
  void operator=(const Singleton &) ATLAS_NOEXCEPT = delete;

  //============================================================================
  // P U B L I C  M E T H O D S

  /**
   * This is the method to use when you want to get the instance of the
   * Singleton.
   */
  static Tp_ &Instance() ATLAS_NOEXCEPT;

 protected:
  //============================================================================
  // P R O T E C T E D   C / D T O R S

  Singleton() ATLAS_NOEXCEPT = default;
};

//==============================================================================
// I N L I N E   M E T H O D S   S E C T I O N

template <class Tp_>
Tp_ &Singleton<Tp_>::Instance() ATLAS_NOEXCEPT {
  static Tp_ instance;
  return instance;
}

}  // namespace sonia_common

#endif  // SONIA_COMMON_PATTERN_SINGLETON_H_

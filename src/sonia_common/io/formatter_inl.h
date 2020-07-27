/**
 * \file	formatter.h
 * \author	Thibaut Mattio <thibaut.mattio@gmail.com>
 * \date	20/02/2016
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

#ifndef SONIA_COMMON_IO_FORMATTER_H_
#error This file may only be included from formatter.h
#endif

namespace sonia_common {

namespace details {

class ArgBase {
 public:
  ArgBase() {}
  virtual ~ArgBase() {}
  virtual void Format(std::ostringstream &ss) = 0;
};

template <class T>
class Arg : public ArgBase {
 public:
  Arg(T arg) : m_arg(arg) {}
  virtual ~Arg() {}
  virtual void Format(std::ostringstream &ss) { ss << m_arg; }

 private:
  T m_arg;
};

class ArgArray : public std::vector<ArgBase *> {
 public:
  ArgArray() {}
  virtual ~ArgArray() {
    std::for_each(begin(), end(), [](ArgBase *p) { delete p; });
  }
};

//------------------------------------------------------------------------------
//
static void FormatItem(std::ostringstream &ss, const std::string &item,
                       const ArgArray &args) {
  size_t index = 0;
  int alignment = 0;

  char *endptr = nullptr;
  index = strtol(&item[0], &endptr, 10);
  if (index >= args.size()) {
    return;
  }

  if (*endptr == ',') {
    alignment = strtol(endptr + 1, &endptr, 10);
    if (alignment > 0) {
      ss << std::right << std::setw(alignment);
    } else if (alignment < 0) {
      ss << std::left << std::setw(-alignment);
    }
  }

  args[index]->Format(ss);

  return;
}

//------------------------------------------------------------------------------
//
template <class T>
static void Transfer(ArgArray &argArray, T t) {
  argArray.push_back(new Arg<T>(t));
}

//------------------------------------------------------------------------------
//
template <class T, typename... Args>
static void Transfer(ArgArray &argArray, T t, Args &&... args) {
  Transfer(argArray, t);
  Transfer(argArray, args...);
}

}  // namespace details

//------------------------------------------------------------------------------
//
template <typename... Args>
ATLAS_INLINE std::string Format(const std::string &format,
                                Args &&... args) ATLAS_NOEXCEPT {
  if (sizeof...(args) == 0) {
    return format;
  }

  details::ArgArray argArray;
  details::Transfer(argArray, args...);
  size_t start = 0;
  size_t pos = 0;
  std::ostringstream ss;
  while (true) {
    pos = format.find('{', start);
    if (pos == std::string::npos) {
      ss << format.substr(start);
      break;
    }

    ss << format.substr(start, pos - start);
    if (format[pos + 1] == '{') {
      ss << '{';
      start = pos + 2;
      continue;
    }

    start = pos + 1;
    pos = format.find('}', start);
    if (pos == std::string::npos) {
      ss << format.substr(start - 1);
      break;
    }

    details::FormatItem(ss, format.substr(start, pos - start), argArray);
    start = pos + 1;
  }

  return ss.str();
}

}  // namespace sonia_common

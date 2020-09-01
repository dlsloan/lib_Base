/* Copyright (C) 2020 David Sloan
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __Base_Exception_h
#define __Base_Exception_h

#include "Base/String.h"

#include <string>

#ifdef NDEBUG
  #define throw_err(val) throw Base::Exception(val)
#else
  #define throw_err(val) throw Base::Exception(val, __FILE__, __LINE__)
#endif // !NDEBUG

#define throw_errno {   \
  int _errno_ = errno;  \
  errno = 0;            \
  throw_err(_errno_);   \
}

#define neg_except(tp, func, ...) ({  \
  tp _ret_ = func(__VA_ARGS__);       \
  if (_ret_ < 0)                      \
    throw_errno;                      \
  _ret_;                              \
})

#define neg_err(tp, func, ...) ({ \
  tp _ret_ = func(__VA_ARGS__);   \
  if (_ret_ < 0)                  \
    throw_err(-_ret_);            \
  _ret_;                          \
})



namespace Base
{
  class Exception : Stringable {
  public:
#ifdef NDEBUG
    Exception(int err);
#else
    Exception(int err, const char *file, int line);
#endif
    Exception(Exception const& err);
    Exception& operator= (Exception const& err);

    virtual String toString() const override;

    virtual ~Exception() {}
  private:
    int err_;
    #ifndef NDEBUG
      const char *file_;
      int line_;
    #endif // !NDEBUG
  };
}

#endif

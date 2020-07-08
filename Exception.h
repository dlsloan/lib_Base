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

namespace Base
{
  class Exception : Stringable {
    public:
    Exception(char const* msg);
    Exception(String const& msg);
    Exception(Exception const& err);
    Exception& operator= (Exception const& err);

    virtual String message() const;
    virtual String toString() const override;

    virtual ~Exception() {}
  private:
    String msg_;
  };
}

#endif

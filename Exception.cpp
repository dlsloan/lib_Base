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

#include "Base/Exception.h"
#include <string.h>

using namespace Base;

Exception::Exception(char const* msg)
{
  msg_ = msg;
}

Exception::Exception(String const& msg)
{
  msg_ = msg;
}
    
Exception::Exception(Exception const& err)
{
  msg_ = err.msg_;
}

Exception& Exception::operator= (Exception const& err)
{
  msg_ = err.msg_;
  return *this;
}

String Exception::message() const
{
  return msg_;
}

String Exception::toString() const
{
  return msg_;
}

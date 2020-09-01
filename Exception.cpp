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
#include <string>

using namespace Base;
using namespace std;

#ifdef NDEBUG
Exception::Exception(int err) :
  err_(err)
{}
    
Exception::Exception(Exception const& err) :
  err_(err.err_)
{}

Exception& Exception::operator= (Exception const& err)
{
  err_ = err.err_;
  return *this;
}

String Exception::toString() const
{
  return strerrno(err_);
}
#else
Exception::Exception(int err, const char *file, int line) :
  err_(err),
  file_(file),
  line_(line)
{}
    
Exception::Exception(Exception const& err) :
  err_(err.err_),
  file_(err.file_),
  line_(err.line_)
{}

Exception& Exception::operator= (Exception const& err)
{
  err_ = err.err_;
  file_ = err.file_;
  line_ = err.line_;
  return *this;
}

String Exception::toString() const
{
  return String(strerror(err_)) + " @ " + file_ + ":" + to_string(line_).c_str();
}
#endif

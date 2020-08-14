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

#ifndef __Base_Hash_h
#define __Base_Hash_h

#include <stdint.h>

namespace Base
{
  template<typename T>
  int hash(T const& value)
  {
    return value.hash();
  }

  template<>
  int hash<uint32_t>(uint32_t const& value);
  template<>
  int hash<int32_t>(int32_t const& value);
  template<>
  int hash<uint16_t>(uint16_t const& value);
  template<>
  int hash<int16_t>(int16_t const& value);
  template<>
  int hash<uint8_t>(uint8_t const& value);
  template<>
  int hash<int8_t>(int8_t const& value);

  class Hash {

  };
}

#endif

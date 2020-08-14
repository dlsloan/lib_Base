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

#ifndef __Base_String_h
#define __Base_String_h

#include "Base/List.h"
#include "Base/compat/stdint.h"

namespace Base {
  class String;

  class Stringable {
    public:
      virtual String toString() const = 0;
      virtual ~Stringable(){}
  };

  String operator+(char const* lhs, String const& rhs);

  class String : public Stringable {
    public:
      String();
      String(char const* value);

      String(String const&);
      String& operator= (String const&);
      String& operator= (char const*);

      ~String();

      String substring(off_t index) const;
      String substring(off_t index, size_t length) const;

      bool contains(String const& value) const;
      bool contains(char const* value) const;

      String replace(char const* find, char const* replace) const;
      String replace(String const& find, String const& replace) const;

      String ltrim();
      String rtrim();
      String trim();

      off_t indexOf(const char* str);
      off_t indexOfR(const char* str);
      off_t indexOf(String const& str) { return indexOf(str.c_str()); }
      off_t indexOfR(String const& str) { return indexOfR(str.c_str()); }

      List<String> split(char const* separator) const;
      List<String> split(String const& separator) const;

      bool startsWith(char const* value) const;
      bool startsWith(String const& value) const;

      bool endsWith(char const* value) const;
      bool endsWith(String const& value) const;

      size_t length() const;
      char const* c_str() const;
      void copyTo(char* charBuffer) const;
      String toString() const override { return *this; }

      int hash() const;

      String operator+(String const& value) const;
      String operator+(char const* value) const;
      String operator+(char value) const;
      friend String operator+(char const* lhs, String const& rhs)
      {
	      return String(lhs, rhs.chars_, rhs.length_);
      }

      String& operator+= (String const& value);
      String& operator+= (char const* value);
      String& operator+= (char value);

      bool operator==(String const& other) const;
      bool operator==(char const* other) const;
      bool operator!=(String const& other) const;
      bool operator!=(char const* other) const;

      char operator[] (const off_t index) const;

    private:
      char* chars_;
      size_t length_;
      size_t size_;

      String(char const* inner1, size_t len1, char const* inner2, size_t len2);
      String(char const* inner1, char const* inner2, size_t len2);
      String(char const* inner1, size_t len1);

      bool partEq(char const* inner, char const* test, size_t testLen) const;
  };
}

#endif

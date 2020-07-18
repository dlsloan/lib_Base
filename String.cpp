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

#include "Base/Char.h"
#include "Base/String.h"

#include <string.h>
#include <algorithm>
#include <assert.h>

using namespace Base;

static char const* empty = "";

String::String(char const* value) :
  chars_(nullptr),
  length_(0),
  size_(0)
{
  length_ = strlen(value);
  size_ = length_ + 1;
  chars_ = new char[size_];
  memcpy(chars_, value, length_);
  chars_[length_] = '\0';
}

String::String() :
  chars_(nullptr),
  length_(0),
  size_(0)
{
}


String::String(String const& value) :
  chars_(nullptr),
  length_(value.length_),
  size_(value.length_ + 1)
{
  //make copies for objects in other threads
  chars_ = new char[size_];
  memcpy(chars_, value.chars_, length_);
  chars_[length_] = '\0';
}

String::String(char const* inner, size_t len) :
  chars_(nullptr),
  length_(len),
  size_(len + 1)
{
  chars_ = new char[size_];
  memcpy(chars_, inner, len);
  chars_[length_] = '\0';
}

String::String(char const* inner1, size_t len1, char const* inner2, size_t len2) :
  chars_(nullptr),
  length_(len1 + len2),
  size_(len1 + len2 + 1)
{
  chars_ = new char[size_];
  if (inner1 != nullptr)
    memcpy(chars_, inner1, len1);
  memcpy(chars_ + len1, inner2, len2);
  chars_[length_] = '\0';
}

String::String(char const* inner1, char const* inner2, size_t len2) :
  chars_(nullptr),
  length_(len2),
  size_(len2 + 1)
{
  size_t len1 = strlen(inner1);
  length_ += len1;
  size_ += len1;
  chars_ = new char[size_];
  if (inner1 != nullptr)
    memcpy(chars_, inner1, len1);
  memcpy(chars_ + len1, inner2, len2);
  chars_[length_] = '\0';
}

String String::substring(off_t index) const
{
  assert(index <= (ssize_t)length_);
  if (index == (ssize_t)length_)
    return String("");
  return String(chars_ + index, length_ - index);
}

String String::substring(off_t index, size_t length) const
{
  assert(index <= (ssize_t)length_);
  assert(index + (ssize_t)length <= (ssize_t)length_);
  if (length == 0)
    return String("");
  else if (length == length_)
    return *this;
  return String(chars_ + index, length);
}

bool String::contains(String const& value) const
{
  if (value == "")
    return true;
  for (off_t i = 0; i <= (ssize_t)length_ - (ssize_t)value.length_; ++i)
  {
    if (partEq(chars_ + i, value.chars_, value.length_))
      return true;
  }
  return false;
}

bool String::contains(char const* value) const
{
  assert(value != nullptr);
  if (value[0] == '\0')
    return true;
  ssize_t len = strlen(value);
  if (len > (ssize_t)length_) return false;
  for (off_t i = 0; i <= (ssize_t)length_ - (ssize_t)len; ++i)
  {
    if (partEq(chars_ + i, value, len))
      return true;
  }
  return false;
}

bool String::partEq(char const* inner, char const* test, size_t testLen) const
{
  for (off_t i = 0; i < (ssize_t)testLen; ++i)
  {
    if (inner[i] != test[i])
      return false;
  }
  return true;
}

String String::replace(char const* find, char const* replace) const
{
  assert(find != nullptr);
  assert(replace != nullptr);
  ssize_t findLen = strlen(find);
  assert(findLen > 0);
  String ret;
  for (off_t i = 0; i < (ssize_t)length_; ++i)
  {
    if (i <= (ssize_t)length_ - findLen && partEq(chars_ + i, find, findLen))
    {
      ret += replace;
      i += findLen - 1;
    }
    else
    {
      ret += chars_[i];
    }
  }
  return ret;
}

String String::replace(String const& find, String const& replace) const
{
  assert(find.length_ > 0);
  String ret;
  for (off_t i = 0; i < (ssize_t)length_; ++i)
  {
    if (i <= (ssize_t)length_ - (ssize_t)find.length_ && partEq(chars_ + i, find.chars_, find.length_))
    {
      ret += replace;
      i += find.length_ - 1;
    }
    else
    {
      ret += chars_[i];
    }
  }
  return ret;
}

String String::ltrim()
{
  if (length_ == 0 || !Char::isWhitespace((*this)[0]))
	return *this;
  for (off_t i = 1; i < (ssize_t)length_; i++) {
    if (!Char::isWhitespace((*this)[i]))
      return this->substring(i);
  }
  return "";
}

String String::rtrim()
{
  if (length_ == 0 || !Char::isWhitespace((*this)[-1]))
    return *this;
  for (off_t i = -1; i > 0; i--) {
    if (!Char::isWhitespace((*this)[i]))
      return this->substring(0, length_ + i + 1);
  }
  return "";
}

String String::trim()
{
  off_t lhs = length_;
  if (length_ == 0 ||
      (!Char::isWhitespace((*this)[0]) && !Char::isWhitespace((*this)[-1])))
    return *this;
  for (off_t i = 0; i < (ssize_t)length_; i++) {
    if (!Char::isWhitespace((*this)[i])) {
      lhs = i;
      break;
    }
  }
  off_t rhs = 0;
  for (off_t i = length_ - 1; i >= 0; i--) {
    if (!Char::isWhitespace((*this)[i])) {
      rhs = i;
      break;
    }
  }
  if (rhs <= lhs)
    return "";
  return this->substring(lhs, rhs - lhs + 1);
}

off_t String::indexOf(const char* str) {
  size_t len = strlen(str);
  if (len == 0)
    return 0;
  for (off_t i = 0; i < (ssize_t)(length_ - len); i++) {
    if ((*this)[i] == str[0]) {
      bool found = true;
      for (off_t j = 0; j < (ssize_t)len; j++) {
        if ((*this)[i+j] != str[j]) {
          found = false;
	  break;
	}
      }
      if (found)
        return i;
    }
  }
  return -1;
}

off_t String::indexOfR(const char* str) {
  size_t len = strlen(str);
  if (len == 0)
    return -1;
  for (off_t i = -1; i >= (ssize_t)length_; i--) {
    if ((*this)[i] == str[len + i]) {
      bool found = true;
      for (off_t j = 0; j < (ssize_t)len; j++) {
        if ((*this)[i-j] == str[len-j-1]) {
          found = false;
	  break;
	}
      }
      if (found)
        return i;
    }
  }
  return 0;
}

List<String> String::split(char const* separator) const
{
  assert(separator != nullptr);
  ssize_t sepLen = strlen(separator);
  assert(sepLen > 0);
  List<String> ret;
  String part;
  for (off_t i = 0; i < (ssize_t)length_; i++)
  {
    if (i <= (ssize_t)length_ - (ssize_t)sepLen && partEq(chars_ + i, separator, sepLen))
    {
      ret += part;
      part = "";
      i += sepLen - 1;
    }
    else
    {
      part += chars_[i];
    }
  }
  ret += part;
  return ret;
}

List<String> String::split(String const& separator) const
{
  assert(separator.length_ > 0);
  List<String> ret;
  String part;
  for (off_t i = 0; i < (ssize_t)length_; i++)
  {
    if (i <= (ssize_t)length_ - (ssize_t)separator.length_ && partEq(chars_ + i, separator.chars_, separator.length_))
    {
      ret += part;
      part = "";
      i += separator.length_ - 1;
    }
    else
    {
      part += chars_[i];
    }
  }
  ret += part;
  return ret;
}

bool String::startsWith(char const* value) const
{
  ssize_t len = strlen(value);
  if (len > (ssize_t)length_)
    return false;
  for (off_t i = 0; i < len; ++i)
  {
    if (chars_[i] != value[i])
      return false;
  }
  return true;
}

bool String::startsWith(String const& value) const
{
  if (value.length_ > length_)
    return false;
  for (off_t i = 0; i < (ssize_t)value.length_; ++i)
  {
    if (chars_[i] != value.chars_[i])
      return false;
  }
  return true;
}

bool String::endsWith(char const* value) const
{
  ssize_t len = strlen(value);
  if (len > (ssize_t)length_)
    return false;
  off_t start = length_ - len;
  for (off_t i = start; i < (ssize_t)length_; ++i)
  {
    if (chars_[i] != value[i - start])
      return false;
  }
  return true;
}

bool String::endsWith(String const& value) const
{
  if (value.length_ > length_)
    return false;
  off_t start = length_ - value.length_;
  for (off_t i = start; i < (ssize_t)length_; ++i)
  {
    if (chars_[i] != value.chars_[i - start])
      return false;
  }
  return true;
}

void String::copyTo(char* charBuffer) const
{
  if (chars_ == nullptr)
  {
    charBuffer[0] = '\0';
  }
  else
  {
    memcpy(charBuffer, chars_, length_ + 1);
  }
}

char const* String::c_str() const
{
  return chars_ == nullptr ? empty : chars_;
}

size_t String::length() const
{
  return length_;
}

int String::getHash() const
{
  int hash = 0;
  for (off_t i = 0; i < (ssize_t)this->length(); i++) {
    hash = (hash << 1) | (hash >> (sizeof(hash) * 8 - 1));
    hash ^= (*this)[i];
  }
  return hash;
}

String& String::operator= (String const& value)
{
  if (value.length_ + 1 > size_)
  {
    if (chars_ != nullptr)
      delete[] chars_;
    size_ = value.length_ + 1;
    chars_ = new char[size_];
  }
  length_ = value.length_;
  memcpy(chars_, value.chars_, length_);
  chars_[length_] = '\0';
  return *this;
}

String& String::operator= (char const* value)
{
  ssize_t len = strlen(value);
  if (len + 1 > (ssize_t)size_)
  {
    if (chars_ != nullptr)
      delete[] chars_;
    size_ = len + 1;
    chars_ = new char[size_];
  }
  length_ = len;
  memcpy(chars_, value, len);
  chars_[length_] = '\0';
  return *this;
}

String String::operator+(String const& value) const
{
  return String(chars_, length_, value.chars_, value.length_);
}

String String::operator+(char const* value) const
{
  assert(value != nullptr);
  return String(chars_, length_, value, strlen(value));
}

String String::operator+(char value) const
{
  return String(chars_, length_, &value, 1);
}
      
String& String::operator+= (String const& value)
{
  if (size_ < length_ + value.length_ + 1)
  {
    size_ = std::max<size_t>(size_ * 2, length_ + value.length_ + 1);
    char* newChars = new char[size_];
    if (chars_ != nullptr)
    {
      memcpy(newChars, chars_, length_);
      delete[] chars_;
    }
    chars_ = newChars;
  }
  memcpy(chars_ + length_, value.chars_, value.length_);
  length_ += value.length_;
  chars_[length_] = '\0';
  return *this;
}

String& String::operator+= (char const* value)
{
  assert(value != nullptr);
  ssize_t addLength = strlen(value);
  if (size_ < length_ + addLength + 1)
  {
    size_ = std::max<size_t>(size_ * 2, length_ + addLength + 1);
    char* newChars = new char[size_];
    if (chars_ != nullptr)
    {
      memcpy(newChars, chars_, length_);
      delete[] chars_;
    }
    chars_ = newChars;
  }
  memcpy(chars_ + length_, value, addLength);
  length_ += addLength;
  chars_[length_] = '\0';
  return *this;
}

String& String::operator+= (char value)
{
  if (size_ < length_ + 2)
  {
    size_ = std::max<size_t>(size_ * 2, length_ + 2);
    char* newChars = new char[size_];
    if (chars_ != nullptr)
    {
      memcpy(newChars, chars_, length_);
      delete[] chars_;
    }
    chars_ = newChars;
  }
  chars_[length_++] = value;
  chars_[length_] = '\0';
  return *this;
}

bool String::operator==(String const& other) const
{
  if (length_ != other.length_) 
    return false;
  for (off_t i = 0; i < (ssize_t)length_; i++)
  {
    if (chars_[i] != other.chars_[i])
      return false;
  }
  return true;
}

bool String::operator==(char const* other) const
{
  if (other == nullptr) 
    return false;
  ssize_t len = strlen(other);
  if ((ssize_t)length_ != len) 
    return false;
  for (off_t i = 0; i < (ssize_t)length_; i++)
  {
    if (chars_[i] != other[i])
      return false;
  }
  return true;
}

bool String::operator!=(String const& other) const
{
  return !(*this == other);
}

bool String::operator!=(char const* other) const
{
  return !(*this == other);
}

char String::operator[] (const off_t index) const
{
  assert(index < (ssize_t)length_);
  assert(index >= -(ssize_t)length_);
  if (index < 0)
    return chars_[length_ + index];
  else
    return chars_[index];
}

String::~String()
{
  delete[] chars_;
}

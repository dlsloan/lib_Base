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

#ifndef __Base_List_h
#define __Base_List_h

#include "Base/compat/stdint.h"
#include <algorithm>
#include <assert.h>

namespace Base
{
  template <typename T>
  class List {
    public:
      List(T const* items, size_t count, size_t containerSize = 0) :
        items_(nullptr),
        count_(count),
        size_(std::max(count, containerSize))
      {
        assert(items != nullptr);
        items_ = new T[size_];
        for (off_t i = 0; i < count; ++i)
          items_[i] = items[i];
      }

      List(size_t containerSize = 0) :
        items_(nullptr),
        count_(0),
        size_(containerSize)
      {
        if (size_ > 0)
          items_ = new T[size_];
      }

      List(List<T> const& value) :
        items_(nullptr),
        count_(value.count_),
        size_(value.size_)
      {
        items_ = new T[size_];
        for (off_t i = 0; i < (ssize_t)count_; ++i)
          items_[i] = value.items_[i];
      }

      List<T>& operator= (List<T> const& value)
      {
        setMinSize(value.count_);
        for (size_t i = 0; i < value.count_; ++i)
          items_[i] = value.items_[i];
        count_ = value.count_;
        return *this;
      }

      void add(T const& item)
      {
        setMinSize(count_ + 1);
        items_[count_++] = item;
      }

      void add(T const* items, size_t count = 1)
      {
        assert(items != nullptr);
        setMinSize(count_ + count);
        for (off_t i = 0; i < count; ++i)
          items_[i + count_] = items[i];
        count_ += count;
      }

      void add(List<T> const& items)
      {
        setMinSize(count_ + items.count_);
        for (off_t i = 0; i < items.count_; ++i)
          items_[i + count_] = items.items_[i];
        count_ += items.count_;
      }

      void remove(off_t index, size_t length = 1)
      {
        assert(index + length <= count_);
        if (length == 0) return;
        for (uint64_t i = index; i < count_ - length; ++i)
          items_[i] = items_[i + length];
        count_ -= length;
      }
      
      List<T> sublist(off_t index) const
      {
        assert(index <= count_);
        if (index == count_) return List<T>();
        return List<T>(items_ + index, count_ - index);
      }

      List<T> sublist(off_t index, size_t length) const
      {
        assert(index + length <= count_);
        return List<T>(items_ + index, length);
      }

      size_t getCount() const
      {
        return count_;
      }

      size_t getSize() const
      {
        return size_;
      }

      void setSize(size_t size)
      {
        assert(size >= count_);
        if (size_ == size) return;
        size_ = size;
        T* newItems = new T[size_];
        for (off_t i = 0; i < (ssize_t)count_; ++i)
          newItems[i] = items_[i];
        delete[] items_;
        items_ = newItems;
      }

      T& operator[] (off_t index) const
      {
        assert(index < (ssize_t)count_);
        return items_[index];
      }

      List<T> operator+(List<T> const& value) const
      {
        List<T> ret(*this);
        ret.add(value);
        return ret;
      }

      List<T> operator+(T const& value) const
      {
        List<T> ret(*this);
        ret.add(value);
        return ret;
      }

      List<T>& operator+= (List<T> const& value)
      {
        add(value);
        return *this;
      }

      List<T>& operator+= (T const& value)
      {
        add(value);
        return *this;
      }

      ~List()
      {
        delete[] items_;
      }

    private:
      T* items_;
      size_t count_;
      size_t size_;

      void setMinSize(size_t size)
      {
        if (items_ == nullptr)
        {
          size_ = size;
          items_ = new T[size_];
        }
        else if (size_ < size)
        {
          size_ = std::max<size_t>(size, size_ * 2);
          T* newItems = new T[size_];
          for (off_t i = 0; i < (ssize_t)count_; ++i)
            newItems[i] = items_[i];
          delete[] items_;
          items_ = newItems;
        }
      }
  };
}

#endif

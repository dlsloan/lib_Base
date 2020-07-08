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

#ifndef __Base_Stack_h
#define __Base_Stack_h

#include "Base/List.h"

namespace Base
{
  template <typename T>
  class Stack {
    public:
      Stack(T const* items, size_t count, size_t containerSize = 0) :
        items_(nullptr),
        count_(count),
        size_(std::max(count, containerSize))
      {
        assert(items != nullptr);
        items_ = new T[size_];
        for (off_t i = 0; i < count; ++i)
          items_[i] = items[i];
      }

      Stack(size_t containerSize = 0) :
        items_(nullptr),
        count_(0),
        size_(containerSize)
      {
        if (size_ > 0)
          items_ = new T[size_];
      }

      Stack(Stack<T> const& value) :
        items_(nullptr),
        count_(value.count_),
        size_(value.size_)
      {
        items_ = new T[size_];
        for (off_t i = 0; i < count_; ++i)
          items_[i] = value.items_[i];
      }

      Stack(List<T> const& value) :
        items_(nullptr),
        count_(value.getCount()),
        size_(value.getSize())
      {
        items_ = new T[size_];
        for (off_t i = 0; i < count_; ++i)
          items_[i] = value[i];
      }

      Stack<T>& operator= (Stack<T> const& value)
      {
        setMinSize(value.count_);
        for (off_t i = 0; i < value.count_; ++i)
          items_[i] = value.items_[i];
        count_ = value.count_;
        return *this;
      }

      void push(T const& item)
      {
        setMinSize(count_ + 1);
        items_[count_++] = item;
      }

      void push(T const* items, size_t count = 1)
      {
        assert(items != nullptr);
        setMinSize(count_ + count);
        for (off_t i = 0; i < count; ++i)
          items_[i + count_] = items[i];
        count_ += count;
      }

      void push(List<T> const& items)
      {
        setMinSize(count_ + items.getCount());
        for (off_t i = 0; i < items.getCount(); ++i)
          items_[i + count_] = items[i];
        count_ += items.getCount();
      }

      T pop()
      {
        assert(count_ > 0);
        count_ -= 1;
        return items_[count_];
      }
      
      List<T> pop(size_t count)
      {
        List<T> list(count);
        pop(list, count);
        return list;
      }
      
      void pop(List<T>& list, size_t count)
      {
        if (count == 0)
          return;
        assert(count <= count_);
        for (off_t i = 0; i < count; ++i)
          list.add(items_[count_ - 1 - i]);
        count_ -= count;
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
        for (off_t i = 0; i < count_; ++i)
          newItems[i] = items_[i];
        delete[] items_;
        items_ = newItems;
      }

      T& operator[] (off_t index) const
      {
        assert(index < count_);
        return items_[count_ - 1 - index];
      }

      Stack<T> operator+(List<T> const& value) const
      {
        Stack<T> ret(*this);
        ret.push(value);
        return ret;
      }

      Stack<T> operator+(T const& value) const
      {
        Stack<T> ret(*this);
        ret.push(value);
        return ret;
      }

      Stack<T>& operator+= (List<T> const& value)
      {
        push(value);
        return *this;
      }

      Stack<T>& operator+= (T const& value)
      {
        push(value);
        return *this;
      }

      ~Stack()
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
          size_ = std::max(size, size_ * 2);
          T* newItems = new T[size_];
          for (off_t i = 0; i < count_; ++i)
            newItems[i] = items_[i];
          delete[] items_;
          items_ = newItems;
        }
      }
  };
}

#endif

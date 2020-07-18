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

#ifndef __Base_Queue_h
#define __Base_Queue_h

#include "Base/List.h"
#include <algorithm>

//TODO: this has only been converted from lists in the constructor!
namespace Base
{
  template <typename T>
  class Queue {
    public:
      Queue(T const* items, size_t count, size_t containerSize = 0) :
        items_(nullptr),
        count_(count),
        size_(std::max(count, containerSize)),
        first_(0)
      {
        assert(items != nullptr);
        items_ = new T[size_];
        for (off_t i = 0; i < count; ++i)
          items_[i] = items[i];
      }

      Queue(size_t containerSize = 0) :
        items_(nullptr),
        count_(0),
        size_(containerSize),
        first_(0)
      {
        if (size_ > 0)
          items_ = new T[size_];
      }

      Queue(Queue<T> const& value) :
        items_(nullptr),
        count_(value.count_),
        size_(value.size_),
        first_(value.first_)
      {
        items_ = new T[size_];
        for (off_t i = 0; i < count_; ++i)
          items_[i] = value[i];
      }

      Queue(List<T> const& value) :
        items_(nullptr),
        count_(value.getCount()),
        size_(value.getSize()),
        first_(0)
      {
        items_ = new T[size_];
        for (off_t i = 0; i < count_; ++i)
          items_[i] = value[i];
      }

      Queue<T>& operator= (Queue<T> const& value)
      {
        setMinSize(value.count_);
        first_ = 0;
        for (off_t i = 0; i < value.count_; ++i)
          items_[i] = value[i];
        count_ = value.count_;
        return *this;
      }

      void enqueue(T const& item)
      {
        setMinSize(count_ + 1);
        off_t pos = first_ + (count_++);
        if (pos >= (ssize_t)size_) pos -= size_;
        items_[pos] = item;
      }

      void enqueue(T const* items, size_t count = 1)
      {
        assert(items != nullptr);
        setMinSize(count_ + count);
        for (off_t i = 0; i < count; ++i)
        {
          off_t pos = first_ + i + count_;
          if (pos >= size_) pos -= size_;
          items_[pos] = items[i];
        }
        count_ += count;
      }

      void enqueue(List<T> const& items)
      {
        setMinSize(count_ + items.getCount());
        for (off_t i = 0; i < items.getCount(); ++i)
        {
          off_t pos = first_ + i + count_;
          if (pos >= size_) pos -= size_;
          items_[pos] = items[i];
        }
        count_ += items.getCount();
      }

      T dequeue()
      {
        assert(count_ > 0);
        T val = items_[first_];
        first_ += 1;
        if (first_ >= (ssize_t)size_) first_ = 0;
        count_ -= 1;
        return val;
      }

      List<T> dequeue(size_t count)
      {
        List<T> list(count);
        dequeue(list, count);
        return list;
      }

      void dequeue(List<T>& list, size_t count)
      {
        assert(count <= count_);
        size_t len = std::min(count, size_ - first_);
        list.add(items_ + first_, len);
        list.add(items_, count - len);
        first_ += count;
        if (first_ >= size_) first_ -= size_;
        count_ -= count;
      }

      size_t count() const
      {
        return count_;
      }

      size_t size() const
      {
        return size_;
      }

      void size(size_t size)
      {
        assert(size >= count_);
        if (size_ == size) return;
        size_ = size;
        T* newItems = new T[size_];
        for (off_t i = 0; i < count_; ++i)
          newItems[i] = (*this)[i];
        delete[] items_;
        items_ = newItems;
        first_ = 0;
      }

      T& operator[] (off_t index) const
      {
        assert(index < (ssize_t)count_);
        off_t pos = index + first_;
        if (pos >= (ssize_t)size_) pos -= size_;
        return items_[pos];
      }

      Queue<T> operator+(List<T> const& value) const
      {
        Queue<T> ret(*this);
        ret.enqueue(value);
        return ret;
      }

      Queue<T> operator+(T const& value) const
      {
        Queue<T> ret(*this);
        ret.enqueue(value);
        return ret;
      }

      Queue<T>& operator+= (List<T> const& value)
      {
        enqueue(value);
        return *this;
      }

      Queue<T>& operator+= (T const& value)
      {
        enqueue(value);
        return *this;
      }

      ~Queue()
      {
        delete[] items_;
      }

    private:
      T* items_;
      size_t count_;
      size_t size_;
      off_t first_;

      void setMinSize(uint64_t size)
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
          for (uint64_t i = 0; i < count_; ++i)
            newItems[i] = (*this)[i];
          delete[] items_;
          first_ = 0;
          items_ = newItems;
        }
      }
  };
}
#endif

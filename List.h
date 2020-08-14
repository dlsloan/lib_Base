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
#include <stdlib.h>

namespace Base
{
  template <typename T>
  class ListIter;

  template <typename T>
  class List {
    public:
      List(T const* items, size_t count, size_t containerSize = 0) :
        items_(nullptr),
        count_(count),
        size_(std::max(count, containerSize))
      {
        assert(items != nullptr);
        items_ = (T*)malloc(sizeof(T) * size_);
        if (items_ == nullptr)
          throw std::bad_alloc();
        for (off_t i = 0; i < (ssize_t)count; ++i) {
          try {
            new (&items_[i])T(items[i]);
          } catch (...) {
            for (off_t j = 0; j < i; j++)
              items_[j].~T();
            free(items_);
            throw;
          }
        }
      }

      List(size_t containerSize = 0) :
        items_(nullptr),
        count_(0),
        size_(containerSize)
      {
        if (size_ > 0) {
          items_ = (T*)malloc(sizeof(T) * size_);
          if (items_ == nullptr)
            throw std::bad_alloc();
        }
      }

      List(List<T> const& value) :
        items_(nullptr),
        count_(value.count_),
        size_(value.size_)
      {
        items_ = (T*)malloc(sizeof(T) * size_);
        if (items_ == nullptr)
          throw std::bad_alloc();
        for (off_t i = 0; i < (ssize_t)count_; ++i) {
          try {
            new (&items_[i])T(value.items_[i]);
          } catch (...) {
            for (off_t j = 0; j < i; j++)
              items_[j].~T();
            free(items_);
            throw;
          }
        }
      }

      List<T>& operator= (List<T> const& value)
      {
        minSize(value.count_);
        off_t i;
        for (i = 0; i < (ssize_t)count_ && i < (ssize_t)value.count_; ++i)
          items_[i] = value.items_[i];
        for (; i < (ssize_t)value.count_; i++) {
          try {
            new (&items_[i])T(value.items_[i]);
          } catch (...) {
            for (off_t j = count_; j < i; j++)
              items_[j].~T();
            throw;
          }
        }
        for (; i < (ssize_t)count_; i++)
          items_[i].~T();
        count_ = value.count_;
        return *this;
      }

      void add(T const& item)
      {
        minSize(count_ + 1);
        new (&items_[count_])T(item);
        count_++;
      }

      void add(T const* items, size_t count = 1)
      {
        assert(items != nullptr);
        minSize(count_ + count);
        for (off_t i = 0; i < count; ++i) {
          try {
            new (&items_[i + count_])T(items[i]);
          } catch (...) {
            for (off_t j = 0; j < i; j++)
              items_[j + count_].~T();
            throw;
          }
        }
        count_ += count;
      }

      void add(List<T> const& items)
      {
        minSize(count_ + items.count_);
        for (off_t i = 0; i < (ssize_t)items.count_; ++i) {
          try {
            new (&items_[i + count_])T(items.items_[i]);
          } catch (...) {
            for (off_t j = 0; j < i; j++)
              items_[j + count_].~T();
            throw;
          }
        }
        count_ += items.count_;
      }

      void remove(off_t index, size_t length = 1)
      {
        assert(index + length <= count_);
        if (length == 0) return;
        for (off_t i = index; i < (ssize_t)(count_ - length); ++i)
          items_[i] = items_[i + length];
        for (off_t i = count_ - length; i < (ssize_t)count_; i++)
          items_[i].~T();
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
        T* newItems = (T*)malloc(sizeof(T) * size);
        if (newItems == nullptr)
          throw std::bad_alloc();
        for (off_t i = 0; i < (ssize_t)count_; ++i) {
          try {
            new (&newItems[i])T(items_[i]);
          } catch (...) {
            for (off_t j = 0; j < i; j++)
              newItems[j].~T();
            free(newItems);
            throw;
          }
        }
        for (off_t i = 0; i < (ssize_t)count_; i++)
          items_[i].~T();
        free(items_);
        items_ = newItems;
        size_ = size;
      }

      T& operator[] (off_t index) const
      {
        assert(index < (ssize_t)count_);
        assert(index >= -(ssize_t)count_);
        if (index < 0)
          return items_[count_ + index];
        else
          return items_[index];
      }

      ListIter<T> iter() const
      {
        return ListIter<T>(*this);
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
        for (off_t i = 0; i < (ssize_t)count_; i++)
          items_[i].~T();
        free(items_);
      }

    private:
      friend class ListIter<T>;

      T* items_;
      size_t count_;
      size_t size_;

      void minSize(size_t size)
      {
        if (items_ == nullptr || size_ < size) {
          size = std::max<size_t>(size, size_ * 2 + 1);
          this->size(size);
        }
      }
  };

  template <typename T>
  class ListIter {
    public:
      off_t i;

      ListIter(List<T> const& lst) :
        i(0),
        lst_(&lst) 
      {}

      bool valid() {
        return i < (ssize_t)lst_->count_;
      }

      T &value() {
        assert(i < (ssize_t)lst_->count_);
        return lst_->items_[i];
      }

      void next() {
        i++;
      }
    private:
      List<T> const* lst_;
  };
}

#endif

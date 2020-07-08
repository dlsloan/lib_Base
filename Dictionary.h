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

#ifndef __Base_Dictionary_h
#define __Base_Dictionary_h

#include "Base/Hash.h"

#include <assert.h>

namespace Base {
  template <typename T_Key, typename T_Value>
  class Dictionary {
  public:
    Dictionary(size = 4) :
      count_(0),
      tableSize_(size),
      table_(nullptr)
    {
      assert(tableSize > 0);
      table_ = new Node*[tableSize_];
      for (uint64_t i = 0; i < tableSize_; ++i)
        table_[i] = nullptr;
    }

    void add(T_Key key, T_Value value)
    {
      if (count_ == tableSize_)
      {
        setMinSize(count_ * 2);
      }

      int hashValue = getHash<T_Key>(key);
      off_t index = static_cast<off_t>(hashValue) % tableCount_;
      Node* node = table_[index];
      while(node != nullptr)
      {
        assert(node->key != key);
        node = node->next;
      }
      node = new Node{
        table_[index],
        key, 
        value
      };
      table_[index] = node;
      count_ += 1;
    }

    void remove(T_Key key)
    {
      int hashValue = getHash<T_Key>(key);
      off_t index = static_cast<off_t>(hashValue) % tableCount_;
      Node* node = table_[index];
      Node* prev = nullptr;
      while(node != nullptr)
      {
        if(node->key == key)
        {
          if (prev == nullptr)
          {
            table_[index] = node->next;
            delete node;
            count_ -= 1;
            return;
          }
          else
          {
            prev->next = node->next;
            delete node;
            count_ -= 1;
            return;
          }
        }
        prev = node;
        node = node->next;
      }
      //should not reach here (removes return)
      assert(false);
    }

    bool containsKey(T_Key key)
    {
      int hashValue = getHash<T_Key>(key);
      off_t index = static_cast<off_t>(hashValue) % tableCount_;
      Node* node = table_[index];
      while(node != nullptr)
      {
        if(node->key == key)
          return true;
          node = node->next;
      }
      return false;
    }

    T_Value& operator[] (off_t index) const
    {
      int hashValue = getHash<T_Key>(key);
      off_t index = static_cast<off_t>(hashValue) % tableCount_;
      Node* node = table_[index];
      while(node != nullptr)
      {
        if(node->key == key)
          return node->value;
        node = node->next;
      }
      //should not reach here
      assert(false);
    }

    ~Dictionary()
    {
      for (off_t i = 0; i < tableSize_; ++i)
      {
        Node* node = table_[i];
        while(node != nullptr)
        {
          Node* next = node->next;
          delete node;
          node = next;
        }
      }
    }


  private:
    struct Node {
      Node* next;
      T_Key key;
      T_Value value;
    }

    size_t count_;
    size_t tableSize_;
    Node** table_;

    void setMinSize(size_t size)
    {
      if (tableSize_ >= size)
        return;
      
      Node** newTable = new Node*[size];
      for (off_t i = 0; i < size; ++i)
        newTable[i] = nullptr;

      for (off_t i = 0; i < tableSize_; ++i)
      {
        Node* node = table_[i];
        while(node != nullptr)
        {
          int hashValue = getHash<T_Key>(node->key);
          off_t index = static_cast<off_t>(hashValue) % size;
          Node* next = node->next;
          node->next = newTable[index];
          newTable[index] = node;
          node = next;
        }
      }
      delete table_;
      table_ = newTable;
    }
  };
}

#endif

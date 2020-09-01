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
#include "Base/List.h"
#include <assert.h>

namespace Base {
  template <typename T_Key, typename T_Value>
  class DictionaryIter;

  template <typename T_Key, typename T_Value>
  class Dictionary {
  public:
  class KVP {
    public:
      T_Key const& key;
      T_Value& value;

      KVP(T_Key const& key, T_Value& value) :
        key(key),
        value(value)
      {}
  };

    Dictionary(size_t size = 4) :
      count_(0),
      tableSize_(size),
      table_(nullptr)
    {
      assert(tableSize_ > 0);
      table_ = new Node*[tableSize_];
      for (off_t i = 0; i < (ssize_t)tableSize_; ++i)
        table_[i] = nullptr;
    }

    Dictionary(Dictionary<T_Key, T_Value> const& dict) :
      count_(0),
      tableSize_(dict.count() < 4 ? 4 : dict.count()),
      table_(nullptr)
    {
      
      assert(tableSize_ > 0);
      table_ = new Node*[tableSize_];
      for (off_t i = 0; i < (ssize_t)tableSize_; ++i)
        table_[i] = nullptr;

      for (auto it = dict.iter(); it.valid(); it.next()) {
        add(it.value().key, it.value().value);
      }
    }
    
    Dictionary<T_Key, T_Value>& operator= (Dictionary<T_Key, T_Value> const& dict)
    {
      this->~Dictionary<T_Key, T_Value>();
      new(this)Dictionary<T_Key, T_Value>(dict);
      return *this;
    }

    void add(T_Key const& key, T_Value const& value)
    {
      assert(!containsKey(key));

      if (count_ == tableSize_)
        minSize(count_ * 2);

      int hashValue = hash<T_Key>(key);
      off_t index = static_cast<off_t>(hashValue) % tableSize_;
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

    void remove(T_Key const& key)
    {
      int hashValue = hash<T_Key>(key);
      off_t index = static_cast<off_t>(hashValue) % tableSize_;
      Node* node = table_[index];
      Node* prev = nullptr;
      while(node != nullptr) {
        if(node->key == key) {
          if (prev == nullptr) {
            table_[index] = node->next;
            delete node;
            count_ -= 1;
            return;
          } else {
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

    bool containsKey(T_Key const& key) const
    {
      int hashValue = hash<T_Key>(key);
      off_t index = static_cast<off_t>(hashValue) % tableSize_;
      Node* node = table_[index];
      while(node != nullptr) {
        if(node->key == key)
          return true;
        node = node->next;
      }
      return false;
    }

    size_t count() const {
      return count_;
    }

    Base::List<T_Key> keys() const {
      Base::List<T_Key> keys_ret(count());
      for (off_t i = 0; i < (ssize_t)tableSize_; i++) {
        if (table_[i] != nullptr) {
          Node* node = table_[i];
          while (node != nullptr) {
                  keys_ret.add(node->key);
                  node = node->next;
          }
        }
      }
      return keys_ret;
    }

    T_Value& operator[] (T_Key const& key) const
    {
      int hashValue = hash<T_Key>(key);
      off_t index = static_cast<off_t>(hashValue) % tableSize_;
      Node* node = table_[index];
      while(node != nullptr) {
        if(node->key == key)
          break;
        node = node->next;
      }
      assert(node != nullptr);
      return node->value;
    }

    DictionaryIter<T_Key, T_Value> iter() const {
      return DictionaryIter<T_Key, T_Value>(*this);
    }

    ~Dictionary()
    {
      for (off_t i = 0; i < (ssize_t)tableSize_; ++i)
      {
        Node* node = table_[i];
        while(node != nullptr)
        {
          Node* next = node->next;
          delete node;
          node = next;
        }
      }
      delete[] table_;
    }

  private:
    struct Node {
      Node* next;
      T_Key key;
      T_Value value;
    };

    size_t count_;
    size_t tableSize_;
    Node** table_;

    friend class DictionaryIter<T_Key, T_Value>;

    void size(size_t size)
    {
      assert(size >= count_);

      Node** newTable = new Node*[size];
      for (off_t i = 0; i < (ssize_t)size; ++i)
        newTable[i] = nullptr;

      for (off_t i = 0; i < (ssize_t)tableSize_; ++i)
      {
        Node* node = table_[i];
        while(node != nullptr)
        {
          int hashValue = hash<T_Key>(node->key);
          off_t index = static_cast<off_t>(hashValue) % size;
          Node* next = node->next;
          node->next = newTable[index];
          newTable[index] = node;
          node = next;
        }
      }
      delete[] table_;
      table_ = newTable;
      tableSize_ = size;
    }

    void minSize(size_t size)
    {
      if (tableSize_ >= size)
        return;
      this->size(size);
    }
  };

  template <typename T_Key, typename T_Value>
  class DictionaryIter {
    public:
      DictionaryIter(Dictionary<T_Key, T_Value> const& dict) :
        i_(0),
        node_(nullptr),
        dict_(&dict)
      {
        for (; i_ < (ssize_t)dict_->tableSize_; i_++) {
          if (dict_->table_[i_] != nullptr) {
            node_ = dict_->table_[i_];
            return;
          }
        }
      }

      void next()
      {
        if (node_ == nullptr)
          return;
        if (node_->next != nullptr) {
          node_ = node_->next;
          return;
        }
        for (i_++; i_ < (ssize_t)dict_->tableSize_; i_++) {
          if (dict_->table_[i_] != nullptr) {
            node_ = dict_->table_[i_];
            return;
          }
        }
        node_ = nullptr;
      }

      bool valid() const {
        return node_ != nullptr;
      }

      typename Dictionary<T_Key, T_Value>::KVP value() const
      {
        return typename Dictionary<T_Key, T_Value>::KVP(node_->key, node_->value);
      }
    private:
      off_t i_;
      typename Dictionary<T_Key, T_Value>::Node* node_;
      Dictionary<T_Key, T_Value> const* dict_;
  };
}

#endif

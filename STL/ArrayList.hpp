#pragma once
#include "Iterator.hpp"
#include <cstddef>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <mutex>
#include <type_traits>

constexpr size_t alignment = 64;

template <class T>
class ArrayList
{
  private:
    static_assert(std::is_destructible_v<T>, "Type T should be destructible");

    using value_type = T;
    using reference_type = T &;
    using const_reference = T const &;
    using pointer_type = T *;
    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;

    static constexpr size_t type_size = sizeof(T);
    static constexpr bool is_trivial = std::is_trivially_copyable_v<T>;

    size_t _capacity = 1;
    T *_storage = nullptr;
    size_t _size = 0;
    void destroy_range(size_t start, size_t end)
    {
        if constexpr (!is_trivial) {
            for (size_t i = start; i < end; i++)
                _storage[i].~T();
        }
    }

  public:
    // Constructors
    ArrayList()
    {
        _storage = allocate(_capacity);
    }
    ArrayList(int capacity) : _capacity(capacity)
    {
        _storage = allocate(_capacity);
    }
    ArrayList(std::initializer_list<T> list) : _size(0), _capacity(list.size())
    {
        _storage = allocate(_capacity);
        for (const T &elem : list)
            push_back(elem);
    }
    ArrayList(ArrayList &&other) noexcept
        : _storage(other._storage), _size(other._size), _capacity(other._capacity)
    {
        other._storage = nullptr;
        other._size = 0;
        other._capacity = 0;
    }
    ~ArrayList()
    {
        destroy_range(0, _size);
        ::operator delete(_storage);
    }

    // Methods
    void push_back(const T &elem)
    {
        if (_size >= _capacity)
            realloc(_capacity * 2);
        new (_storage + _size) T(elem);
        _size++;
    }

    void push_back(T &&elem)
    {
        if (_size >= _capacity)
            realloc(_capacity * 2);
        new (_storage + _size) T(std::move(elem));
        _size++;
    }

    value_type pop_back()
    {
        if (_size == 0)
            throw std::out_of_range("pop from empty arraylist");
        T temp = _storage[_size - 1];
        _storage[_size - 1].~T();
        _size--;
        if (_size < _capacity / 2)
            realloc(_capacity / 2);
        return temp;
    }

    void resize(size_t new_size)
    {
        if (new_size > _capacity)
            realloc(new_size);
        if (new_size > _size) {
            for (size_t i = _size; i < new_size; i++)
                new (_storage + i) T();
        } else {
            destroy_range(new_size, _size);
        }
        _size = new_size;
    }

    void resize(size_t new_size, const T &value)
    {
        if (new_size > _capacity)
            realloc(new_size);
        if (new_size > _size) {
            for (size_t i = _size; i < new_size; i++)
                new (_storage + i) T(value);
        } else {
            destroy_range(new_size, _size);
        }
        _size = new_size;
    }

    void reserve(size_t new_capacity)
    {
        if (_capacity < new_capacity) {
            size_t aligned_capacity = (new_capacity + alignment - 1) & ~(alignment - 1);
            realloc(aligned_capacity);
        }
    }

    void realloc(size_t newCapacity)
    {
        T *newStorage = allocate(newCapacity);
        if constexpr (!is_trivial || std::is_nothrow_move_constructible_v<T>) {
            for (size_t i = 0; i < _size; i++)
                new (newStorage + i) T(std::move(_storage[i]));
            destroy_range(0, _size);
        } else {
            for (size_t i = 0; i < _size; i++)
                new (newStorage + i) T(_storage[i]);
            destroy_range(0, _size);
        }
        ::operator delete(_storage);
        _storage = newStorage;
        _capacity = newCapacity;
    }

    void shrink_to_fit()
    {
        if (_size < _capacity)
            realloc(_size);
    }

    void erase_unordered(size_t index)
    {
        if (index >= _size)
            return;
        if (index != _size - 1) {
            if constexpr (is_trivial)
                _storage[index] = _storage[_size - 1];
            else
                _storage[index] = std::move(_storage[_size - 1]);
        }
        _storage[_size - 1].~T();
        _size--;
    }

    void erase(size_t index)
    {
        if (index >= _size)
            return;
        _storage[index].~T();
        if constexpr (is_trivial) {
            std::memmove(_storage + index, _storage + index + 1, (_size - index - 1) * type_size);
        } else {
            for (size_t i = index; i < _size - 1; i++) {
                new (_storage + i) T(std::move(_storage[i + 1]));
                _storage[i + 1].~T();
            }
        }
        _size--;
    }

    template <class V>
    ArrayList<V> map(std::function<V(T, int)> operation)
    {
        ArrayList<V> list;
        list.reserve(_size);
        for (size_t i = 0; i < _size; i++)
            list.push_back(operation(_storage[i], i));
        return list;
    }

    template <class V>
    V reduce(std::function<V(T, T)> operation, V init)
    {
        V acc = init;
        for (size_t i = 0; i < _size; i++)
            acc = operation(acc, _storage[i]);
        return acc;
    }

    inline bool operator==(const ArrayList &other) const
    {
        if (_size != other._size)
            return false;
        for (size_t i = 0; i < _size; i++)
            if (_storage[i] != other._storage[i])
                return false;
        return true;
    }

    // Getters
    inline T &operator[](size_t idx)
    {
        return _storage[idx];
    }
    inline const T &operator[](size_t idx) const
    {
        return _storage[idx];
    }
    inline size_t size() const
    {
        return _size;
    }

    // Iterators
    iterator begin()
    {
        return iterator(_storage);
    }
    iterator end()
    {
        return iterator(_storage + _size);
    }
    const_iterator begin() const
    {
        return const_iterator(_storage);
    }
    const_iterator end() const
    {
        return const_iterator(_storage + _size);
    }

    void print(const char *separator = "\n")
    {
        for (auto &e : *this)
            if constexpr (std::is_same_v<decltype(std::cout << e), std::ostream &>)
                std::cout << e << separator;
    }

  private:
    static T *allocate(size_t n)
    {
        return (T *)::operator new(n * type_size, std::align_val_t(alignment));
    }
};
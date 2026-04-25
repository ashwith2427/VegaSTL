#ifndef VECTOR_H
#define VECTOR_H
#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator.h>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <class Derived, class T>
class Allocator
{
public:
    T* allocate(size_t n)
    {
        return static_cast<Derived*>(this)->allocate(n);
    }
    template <class... Args>
    void construct(T* ptr, Args&&... args)
    {
        return static_cast<Derived*>(this)->construct(ptr, std::forward<Args>(args)...);
    }
    void destroy(T* ptr)
    {
        static_cast<Derived*>(this)->destroy(ptr);
    }
    void deallocate(T* ptr, size_t n)
    {
        static_cast<Derived*>(this)->deallocate(ptr, n);
    }
};

template <class T>
class BasicAllocator : Allocator<BasicAllocator<T>, T>
{
public:
    using value_type = T;
    T* allocate(size_t n)
    {
        return (T*)::operator new(n * sizeof(T));
    }
    template <class... Args>
    void construct(T* ptr, Args&&... args)
    {
        new (ptr) T(std::forward<Args>(args)...);
    }
    void destroy(T* ptr)
    {
        ptr->~T();
    }
    void deallocate(T* ptr, size_t)
    {
        ::operator delete(ptr);
    }
    template <class U>
    using rebind_alloc = BasicAllocator<U>;
};

template <class T, class Allocator = BasicAllocator<T>>
class vector
{
    using iterator        = Iterator<T>;
    using const_iterator  = Iterator<T const>;
    using value_type      = T;
    using reference       = T&;
    using const_reference = T const&;
    using pointer         = T*;
    using const_pointer   = T const*;
    using traits          = std::allocator_traits<Allocator>;

public:
    vector(size_t capacity = 16)
        : _size(0)
        , _capacity(capacity)
        , _allocator(Allocator())
    {
        _data = traits::allocate(_allocator, _capacity);
    }

    vector(std::initializer_list<T> list)
        : _size(0)
        , _capacity(1)
        , _allocator(Allocator())
    {
        while (_capacity < list.size())
        {
            _capacity *= 2;
        }
        _data = traits::allocate(_allocator, _capacity);
        for (auto const& elem : list)
        {
            push_back(elem);
        }
    }
    vector(vector const& other)
        : _size(other._size)
        , _capacity(other._capacity)
        , _allocator(other._allocator)
    {
        _data = traits::allocate(_allocator, _capacity);
        for (size_t i = 0; i < other._size; i++)
        {
            traits::construct(_allocator, _data + i, other[i]);
        }
    }

    vector(vector&& other)
        : _size(other._size)
        , _capacity(other._capacity)
        , _data(other._data)
        , _allocator(std::move(other._allocator))
    {
        other._size     = 0;
        other._capacity = 0;
        other._data     = nullptr;
    }
    ~vector()
    {
        for (size_t i = 0; i < _size; i++)
        {
            traits::destroy(_allocator, _data + i);
        }
        traits::deallocate(_allocator, _data, _capacity);
    }

    vector& operator=(vector other)
    {
        std::swap(_data, other._data);
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
        std::swap(_allocator, other._allocator);
        return *this;
    }

    iterator begin()
    {
        return iterator{_data};
    }

    iterator end()
    {
        return iterator{_data + _size};
    }

    void push_back(T const& element)
    {
        push_back_impl(element);
    }

    void push_back(T&& element)
    {
        push_back_impl(std::move(element));
    }

    void pop_back()
    {
        if (_size == 0)
            throw std::out_of_range("Index out of bounds...");
        traits::destroy(_allocator, _data + _size - 1);
        _size--;
    }

    iterator erase(iterator pos)
    {
        size_t index = pos - begin();
        return erase_impl(index, 1);
    }

    iterator erase(const_iterator pos)
    {
        size_t index = pos - begin();
        return erase_impl(index, 1);
    }

    iterator erase(iterator first, iterator second)
    {
        size_t count = second - first;
        size_t index = first - begin();
        return erase_impl(index, count);
    }

    iterator erase(const_iterator first, const_iterator second)
    {
        size_t count = second - first;
        size_t index = first - begin();
        return erase_impl(index, count);
    }

    iterator erase_unordered(iterator pos)
    {
        size_t index = pos - begin();
        std::swap(_data[index], _data[_size - 1]);
        _size--;
        return iterator{_data + index};
    }

    void reserve(size_t new_capacity)
    {
        if (new_capacity > _capacity)
        {
            reallocate(new_capacity);
        }
    }

    void resize(size_t new_size)
    {
        resize_impl(new_size);
    }

    void resize(size_t new_size, T const& element)
    {
        resize_impl(new_size, element);
    }

    void shrink_to_fit()
    {
        if (_size != _capacity)
        {
            reallocate(_size);
        }
    }

    size_t size() const
    {
        return _size;
    }

    T& operator[](size_t index)
    {
        return _data[index];
    }

    T* data() const
    {
        return _data;
    }

private:
    T*        _data;
    size_t    _size;
    size_t    _capacity;
    Allocator _allocator;
    template <class U>
    void push_back_impl(U&& element)
    {
        if (_size >= _capacity)
        {
            reallocate(_capacity * 2);
        }
        traits::construct(_allocator, _data + _size, std::forward<U>(element));
        _size++;
    }
    void reallocate(size_t new_capacity)
    {
        T* new_data = traits::allocate(_allocator, new_capacity);
        if constexpr (std::is_trivially_copyable_v<T>)
        {
            std::memmove(new_data, _data, sizeof(T) * _size);
        }
        else
        {
            std::uninitialized_move(_data, _data + _size, new_data);
            for (size_t i = 0; i < _size; i++)
            {
                traits::destroy(_allocator, _data + i);
            }
        }
        traits::deallocate(_allocator, _data, _capacity);
        _data     = new_data;
        _capacity = new_capacity;
    }
    iterator erase_impl(size_t index, size_t count)
    {
        if (index >= _size)
            return end();
        size_t last         = std::min(index + count, _size);
        size_t num_to_erase = last - index;
        size_t num_to_move  = _size - last;
        if constexpr (std::is_trivially_copyable_v<T>)
        {
            std::memmove(_data + index, _data + last, num_to_move * sizeof(T));
        }
        else
        {
            for (size_t i = 0; i < num_to_move; ++i)
            {
                _data[index + i] = std::move(_data[last + i]);
            }
            for (size_t i = 0; i < num_to_erase; ++i)
            {
                traits::destroy(_allocator, _data + (_size - 1 - i));
            }
        }
        _size -= num_to_erase;
        return iterator{_data + index};
    }
    void ensure_capacity(size_t n)
    {
        size_t new_capacity = _capacity == 0 ? 16 : _capacity;
        while (new_capacity < n)
        {
            new_capacity *= 2;
        }
        reallocate(new_capacity);
    }
    template <class... Args>
    void resize_impl(size_t new_size, Args&&... args)
    {
        if (_size == new_size)
            return;
        if (new_size > _capacity)
        {
            reallocate(std::max(new_size, _capacity * 2));
        }
        if (new_size > _size)
        {
            for (size_t i = _size; i < new_size; i++)
            {
                traits::construct(_allocator, _data + i, std::forward<Args>(args)...);
            }
        }
        else
        {
            for (size_t i = new_size; i < _size; i++)
            {
                traits::destroy(_allocator, _data + i);
            }
        }
        _size = new_size;
    }
};

#endif

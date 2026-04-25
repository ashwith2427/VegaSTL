#ifndef ITERATOR_H
#define ITERATOR_H

#include <cstddef>
#include <iterator>
template <class T>
class Iterator
{

public:
    using value_type        = T;
    using reference         = T&;
    using const_reference   = T const&;
    using pointer           = T*;
    using const_pointer     = T const*;
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;
    Iterator(T* ptr)
        : _ptr(ptr)
    {
    }
    Iterator(Iterator&& it)
        : _ptr(std::move(it._ptr))
    {
        it._ptr = nullptr;
    }
    Iterator(Iterator const& it)
        : _ptr(it._ptr)
    {
    }
    Iterator& operator=(Iterator it)
    {
        std::swap(_ptr, it._ptr);
        return *this;
    }
    constexpr auto operator<=>(Iterator const&) const = default;

    Iterator& operator++()
    {
        _ptr++;
        return *this;
    }

    Iterator operator++(int)
    {
        Iterator temp = *this;
        _ptr++;
        return temp;
    }

    Iterator& operator--()
    {
        _ptr--;
        return *this;
    }

    Iterator operator--(int)
    {
        Iterator temp = *this;
        _ptr--;
        return temp;
    }

    Iterator operator+(difference_type pos) const
    {
        return Iterator{_ptr + pos};
    }
    Iterator operator-(difference_type pos) const
    {
        return Iterator{_ptr - pos};
    }

    difference_type operator-(Iterator const& it) const
    {
        return _ptr - it._ptr;
    }

    Iterator& operator+=(difference_type pos)
    {
        _ptr += pos;
        return *this;
    }

    Iterator& operator-=(difference_type pos)
    {
        _ptr -= pos;
        return *this;
    }

    constexpr pointer operator->() const
    {
        return _ptr;
    }

    constexpr reference operator*() const
    {
        return *_ptr;
    }

    constexpr reference operator[](difference_type index) const
    {
        return *(_ptr + index);
    }

private:
    T* _ptr;
};

#endif

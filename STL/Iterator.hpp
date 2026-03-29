#include <iterator>

template <class Element>
class Iterator
{
  private:
    Element *ptr;

  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = Element;
    using difference_type = std::ptrdiff_t;
    using pointer = Element *;
    using reference = Element &;
    Iterator(Element *ptr = nullptr) : ptr(ptr)
    {
    }
    Element &operator*()
    {
        return *ptr;
    }
    const Element &operator*() const
    {
        return *ptr;
    }
    Iterator &operator++()
    {
        ptr++;
        return *this;
    }
    Iterator operator++(int)
    {
        Iterator temp = *this;
        ++(*this);
        return temp;
    }
    Iterator operator+(int offset) const
    {
        Iterator newIt(ptr + offset);
        return newIt;
    }
    Iterator operator-(int offset) const
    {
        Iterator newIt(ptr - offset);
        return newIt;
    }
    ptrdiff_t operator-(Iterator const &other) const
    {
        return ptr - other.ptr;
    }
    Iterator &operator+=(int offset)
    {
        ptr += offset;
        return *this;
    }
    Iterator &operator-=(int offset)
    {
        ptr -= offset;
        return *this;
    }
    bool operator<(const Iterator &other) const
    {
        return ptr < other.ptr;
    }
    bool operator>(const Iterator &other) const
    {
        return ptr > other.ptr;
    }
    bool operator<=(const Iterator &other) const
    {
        return ptr <= other.ptr;
    }
    bool operator>=(const Iterator &other) const
    {
        return ptr >= other.ptr;
    }
    bool operator==(const Iterator &other) const
    {
        return ptr == other.ptr;
    }
    bool operator!=(const Iterator &other) const
    {
        return !(*this == other);
    }
    Element &operator[](int index)
    {
        return *(ptr + index);
    }
};

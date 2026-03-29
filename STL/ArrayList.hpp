#include <iostream>
#include <initializer_list>
#include <type_traits>
#include "Iterator.hpp"

constexpr size_t alignment = 64;

template <class T>
class ArrayList {
private:
    static_assert(std::is_destructible_v<T>, "Type T should destructible");
    using value_type = T;
    using reference_type = T&;
    using const_reference = T const&;
    using pointer_type = T*;
    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;
    static constexpr size_t type_size = sizeof(T);
    size_t _capacity = 1;
    T* _storage;
    size_t _size;
public:
    // Constructors
    ArrayList();
    ArrayList(int capacity);
    ArrayList(std::initializer_list<T> list);
    ArrayList(ArrayList&& other);
    ~ArrayList();
    // Methods
    void push_back(T const& element);
    void push_back(T&& element);
    value_type pop_back();
    void resize(size_t new_size);
    void resize(size_t new_size, const T& value);
    void reserve(size_t new_capacity);
    void realloc(size_t newCapacity);
    void shrink_to_fit();
    template <class V>
    ArrayList<V> map(std::function<V(T, int)> operation);
    template <class V>
    V reduce(std::function<V(T, T)> operation, V init);
    bool operator==(ArrayList const& other);
    // Getters
    inline T const& operator[](size_t idx) const;
    T& operator[](size_t idx);
    inline size_t size() const;
    // Output
    void print(const char* seperator = "\n");
    // Iterators
    iterator begin() {
        return iterator(_storage);
    }
    iterator end() {
        return iterator(_storage + _size);
    }
    const_iterator begin() const {
        return const_iterator(_storage);
    }
    const_iterator end() const {
        return const_iterator(_storage + _size);
    }
};

template <class T>
ArrayList<T>::ArrayList(): _size(0) {
    _storage = (T*)::operator new(type_size * _capacity, std::align_val_t(alignment));
}
template <class T>
ArrayList<T>::ArrayList(int capacity): _size(0), _capacity(capacity) {
    _storage = (T*)::operator new(type_size * capacity, std::align_val_t(alignment));
}
template <class T>
ArrayList<T>::ArrayList(std::initializer_list<T> list): _size(0) {
    _capacity = list.size();
    _storage = (T*)::operator new(type_size * _capacity, std::align_val_t(alignment));

    for (const T& elem : list) {
        new (_storage + _size) T(elem);
        _size++;
    }
}
template <class T>
ArrayList<T>::ArrayList(ArrayList&& other) {
    _storage = other._storage;
    _size = other._size;
    _capacity = other._capacity;
    other._storage = nullptr;
    other._size = 0;
    other._capacity = 0;
}
template <class T>
ArrayList<T>::~ArrayList() {
    for(int i=0;i<_size;i++) {
        _storage[i].~T();
    }
    ::operator delete(_storage);
}

template <class T>
inline void ArrayList<T>::push_back(T const& element) {
    if (_size >= _capacity) {
        realloc(_capacity * 2);
    }
    new (_storage + _size) T(element);
    _size++;
}

template <class T>
inline void ArrayList<T>::push_back(T&& element) {
    if (_size >= _capacity) {
        realloc(_capacity * 2);
    }
    new (_storage + _size) T(std::move(element));
    _size++;
}

template <class T>
ArrayList<T>::value_type ArrayList<T>::pop_back() {
    if(_size == 0) throw std::out_of_range("pop from empty vector");
    T temp = _storage[_size-1];
    _storage[_size-1].~T();
    _size--;
    if(_size < _capacity/2) {
        realloc(_capacity/2);
        _capacity = _capacity/2;
    }
    return temp;
}

template <class T>
void ArrayList<T>::resize(size_t new_size) {
    if(new_size > _capacity) {
        realloc(new_size);
    }
    if(new_size > _size) {
        for(size_t i=_size;i<new_size;i++) {
            new (_storage + i) T();
        }
    } else {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            for(size_t i = new_size;i<_size;i++) {
                _storage[i].~T();
            }
        }
    }
    _size = new_size;
}

template <class T>
void ArrayList<T>::resize(size_t new_size, const T& value) {
    if (new_size > _capacity) {
        realloc(new_size);
    }

    if (new_size > _size) {
        for (size_t i = _size; i < new_size; i++) {
            new (_storage + i) T(value);
        }
    } else {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            for (size_t i = new_size; i < _size; i++) {
                _storage[i].~T();
            }
        }
    }

    _size = new_size;
}

template <class T>
void ArrayList<T>::reserve(size_t new_capacity) {
    if(_capacity < new_capacity) {
        size_t aligned_capacity = (new_capacity + alignment - 1)&~(alignment - 1);
        realloc(aligned_capacity);
    }
}

template <class T>
void ArrayList<T>::realloc(size_t newCapacity) {
    T* newStorage = (T*)operator new(newCapacity * type_size, std::align_val_t(alignment));
    if constexpr(std::is_nothrow_move_constructible_v<T>) {
        for(int i=0;i<_size;i++) {
            new (newStorage + i) T(std::move(_storage[i]));
        }
    } else {
        for(int i=0;i<_size;i++) {
            new (newStorage + i) T(_storage[i]);
        }
    }
    
    for(int i=0;i<_size;i++) {
        _storage[i].~T();
    }
    ::operator delete(_storage);
    _storage = newStorage;
    _capacity = newCapacity;
}

template <class T>
void ArrayList<T>::shrink_to_fit() {
    if(_size < _capacity) {
        realloc(_size);
    }
}

template <class T>
template <class V>
ArrayList<V> ArrayList<T>::map(std::function<V(T, int)> operation) {
    ArrayList<V> list;
    list.reserve(_size);
    for(int i=0;i<_size;i++) {
        list.push_back(operation(this->_storage[i], i));
    }
    return list;
}

template <class T>
template <class V>
V ArrayList<T>::reduce(std::function<V(T, T)> operation, V init) {
    V acc = init;
    for(int i=0;i<_size;i++){
        acc = operation(acc, _storage[i]);
    }
    return acc;
}

template <class T>
bool ArrayList<T>::operator==(ArrayList const& other) {
    if (_size != other._size) return false;
    for(int i=0;i<_size;i++) {
        if(_storage[i] != other._storage[i]) {
            return false;
        }
    }
    return true;
}

template <class T>
inline T const& ArrayList<T>::operator[](size_t idx) const {
    return _storage[idx];
}

template <class T>
T& ArrayList<T>::operator[](size_t idx) {
    return _storage[idx];
}

template<typename T>
inline size_t ArrayList<T>::size() const {
    return _size;
}

template <class T, class = void>
struct is_printable : std::false_type {};

template <class T>
struct is_printable<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>> : std::true_type {};

template <class T>
void ArrayList<T>::print(const char* seperator) {
    for(auto element: *this) {
        if(is_printable<T>::value) {
            std::cout << element << seperator;
        }
    }
}



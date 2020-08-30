#include <utility>
#include <cstddef>

template<class T>
struct Array {
    // все объявленные ниже методы уже реализованы
    explicit Array(size_t size = 0);
    Array(Array const& a);
    Array & operator=(Array const& a);
    ~Array();

    size_t size() const;
    T &         operator[](size_t i);
    T const&    operator[](size_t i) const;
    
    void swap(Array& a) {
        std::swap(size_, a.size_);
        std::swap(data_, a.data_);
        
    }
    // реализуйте перемещающий конструктор
    Array(Array&& a)
        : Array() {
        swap(a);
    }

    // реализуйте перемещающий оператор присваивания
    Array& operator=(Array&& a) {
        swap(a);
        return *this;
    }

private:    
    size_t  size_;
    T *     data_;    
};
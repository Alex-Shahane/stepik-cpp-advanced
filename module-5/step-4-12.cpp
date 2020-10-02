#include <cstddef>
#include <string>
#include <iostream>

struct Struct {
    size_t size = 0;
};

template<typename T, size_t (T::*)() const = &T::size>
size_t get_size(const T& val) {
    return val.size();
}

template<typename T, size_t T::* = &T::size>
size_t get_size(const T& val) {
    return val.size;
}

int main() {
    std::string s{ "Hello" };
    size_t s_size = get_size(s);   // 5, вызывается метод size()
    std::cout << s_size << std::endl;

    Struct x{ 10 };
    size_t x_size = get_size(x);
    std::cout << x_size << std::endl;

    return 0;
}

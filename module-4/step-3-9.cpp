#include <iostream>
#include <string>
#include <utility> // std::declval

template<class T>
void do_math() noexcept(noexcept(std::declval<T>() + std::declval<T>()) &&
                        noexcept(T(std::declval<T>())) &&
                        noexcept(std::declval<T&>() = std::declval<T>()) &&
                        noexcept(T(std::declval<T&>())) &&
                        noexcept(std::declval<T&>() = std::declval<T&>()) ) {
    // тело функции нужно оставить пустым
}

int main() {
    bool b1 = noexcept(do_math<int>()); // true
    std::cout << b1 << std::endl;

    bool b2 = noexcept(do_math<std::string>()); // false
    std::cout << b2 << std::endl;

    return 0;
}

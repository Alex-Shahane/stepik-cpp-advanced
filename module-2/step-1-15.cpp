#include <iostream>
#include <typeinfo>


void print_values(std::ostream& os) {
}

template<typename T, typename... Args>
void print_values(std::ostream& os, const T& value, const Args&... args) {
    os << typeid(value).name() << ": " << value << std::endl;
    print_values(os, args...);
}

int main() {
    print_values(std::cout, 0, 3.5, "Hello");

    return 0;
}
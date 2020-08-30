#include <utility> // для std::move и std::forward
#include <string>

template<typename T, typename... Args>
auto apply (T func, Args&&... args) -> decltype(func(std::forward<Args>(args)...)) {
    return func(std::forward<Args>(args)...) ; 
}

int main() {
    auto fun = [](std::string a, std::string const& b) { return a += b; };

    std::string s("world!");

// s передаётся по lvalue ссылке,
// а временный объект по rvalue ссылке 
    s = apply(fun, std::string("Hello, "), s);

    return 0;
}
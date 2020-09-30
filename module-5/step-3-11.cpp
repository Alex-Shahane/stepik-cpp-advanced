#include <cstddef>
#include <tuple>
#include <type_traits>

template<int... Ints>
struct IntList;

template<int H, int... Ints>
struct IntList<H, Ints...> {
    static int const Head = H;
    using Tail = IntList<Ints...>;
};

template<>
struct IntList<> { };

template<typename IL>
struct Length { 
    static int const value = 1 + Length<typename IL::Tail>::value; 
};

template<>
struct Length<IntList<>> {
    static int const value = 0;
};

template<int H, typename IL>
struct IntCons;

template<int H, int... Ints>
struct IntCons<H, IntList<Ints...>> {
    using type = IntList<H, Ints...>;
};

template<int N, int X = 0>
struct Generate {
    using type = typename IntCons<X, typename Generate<N - 1, X + 1>::type>::type;
};

template<int X>
struct Generate<0, X> {
    using type = IntList<>;
};

template<typename F, typename... Args, int... I>
auto apply_impl(F f, const std::tuple<Args...>& t, IntList<I...>) -> typename std::result_of<F(Args...)>::type {
    return f(std::get<I>(t)...);
}

template<typename F, typename... Args>
auto apply(F f, const std::tuple<Args...>& t) -> typename std::result_of<F(Args...)>::type {
    return apply_impl(f, t, typename Generate<sizeof...(Args)>::type{});
}

template<typename Il1, typename Il2, template<int, int> class F>
struct Zip {
    using type = typename IntCons<F<Il1::Head, Il2::Head>::value, typename Zip<typename Il1::Tail, typename Il2::Tail, F>::type>::type;
};

template<template<int, int> class F>
struct Zip<IntList<>, IntList<>, F> {
    using type = IntList<>;
};

template<int a, int b>
struct Plus {
    static int const value = a + b;    
};

int main() {
    using primes = IntList<2, 3, 5, 7, 11, 13>;
    constexpr int head = primes::Head;
    using odd_primes = primes::Tail;

    constexpr size_t len = Length<primes>::value; // 6

    using L1 = IntList<2, 3, 4>;

    using L2 = IntCons<1, L1>::type;   // IntList<1,2,3,4>

    using L3 = Generate<5>::type;      // IntList<0,1,2,3,4>

    auto f = [](int x, double y, double z) { return x + y + z; };
    auto t = std::make_tuple(30, 5.0, 1.6);  // std::tuple<int, double, double>
    auto res = apply(f, t);                // res = 36.6


    // два списка одной длины
    using L4 = IntList<1,2,3,4,5>;
    using L5 = IntList<1,3,7,7,2>;

    // результат применения — список с поэлементными суммами
    using L6 = Zip<L4, L5, Plus>::type;  // IntList<2, 5, 10, 11, 7>

    return 0;
}

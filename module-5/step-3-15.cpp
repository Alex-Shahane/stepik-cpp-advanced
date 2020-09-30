#include <cstddef>
#include <tuple>
#include <type_traits>
#include <iostream>

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

template<int a, int b>
struct Minus {
    static int const value = a - b;
};

template<int m = 0, int kg = 0, int s = 0, int A = 0, int K = 0, int mol = 0, int cd = 0>
using Dimension = IntList<m, kg, s, A, K, mol, cd>;

template<typename IL1>
struct Quantity {
    Quantity() {}

    explicit Quantity(double data)
        : data_(data)
    { }

    double value() const {
        return data_;
    }

    Quantity operator+(const Quantity& other) {
        return Quantity<IL1>(data_ + other.value());
    }

    Quantity operator-(const Quantity& other) {
        return Quantity<IL1>(data_ - other.value());
    }

    template<typename IL2>
    Quantity<typename Zip<IL1, IL2, Plus>::type> operator*(const Quantity<IL2>& other) {
        return Quantity<typename Zip<IL1, IL2, Plus>::type>(data_ * other.value());
    }

    Quantity operator*(double right) {
        return Quantity<IL1>(data_ * right);
    }

    template<typename IL2>
    Quantity<typename Zip<IL1, IL2, Minus>::type> operator/(const Quantity<IL2>& other) {
        return Quantity<typename Zip<IL1, IL2, Minus>::type>(data_ / other.value());
    }

    Quantity operator/(double right) {
        return Quantity<IL1>(data_ / right);
    }

private:
    double data_;
};

template<typename IL1>
Quantity<IL1> operator*(double left, const Quantity<IL1>& right) {
    return Quantity<IL1>(left * right.value());
}

template<typename IL1>
Quantity<typename Zip<Dimension<>, IL1, Minus>::type> operator/(double left, const Quantity<IL1>& right) {
    return Quantity<Dimension<>>(left) / right;
}


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
    using L4 = IntList<1, 2, 3, 4, 5>;
    using L5 = IntList<1, 3, 7, 7, 2>;

    // результат применения — список с поэлементными суммами
    using L6 = Zip<L4, L5, Plus>::type;  // IntList<2, 5, 10, 11, 7>
    
    using NumberQ = Quantity<Dimension<>>;          // число без размерности
    using LengthQ = Quantity<Dimension<1>>;          // метры
    using MassQ = Quantity<Dimension<0, 1>>;       // килограммы
    using TimeQ = Quantity<Dimension<0, 0, 1>>;    // секунды
    using VelocityQ = Quantity<Dimension<1, 0, -1>>;   // метры в секунду
    using AccelQ = Quantity<Dimension<1, 0, -2>>;   // ускорение, метры в секунду в квадрате
    using ForceQ = Quantity<Dimension<1, 1, -2>>;   // сила в ньютонах

    LengthQ   l{ 30000 };      // 30 км
    // TimeQ     t{ 10 * 60 };    // 10 минут
    // вычисление скорости
//    VelocityQ v = l / t;     // результат типа VelocityQ, 50 м/с

    AccelQ    a{ 9.8 };        // ускорение свободного падения
    MassQ     m{ 80 };         // 80 кг
    // сила притяжения, которая действует на тело массой 80 кг
    ForceQ    ff = m * a;     // результат типа ForceQ

    return 0;
}

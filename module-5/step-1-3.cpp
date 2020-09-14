#include <iostream>
#include <future>
#include <vector>

template<typename It, typename F1, typename F2>
auto map_reduce_impl(It p, It q, F1 f1, F2 f2) -> decltype(f2(f1(*p), f1(*p))) {
    auto res = f1(*p++);
    for (; p != q; ++p)
        res = f2(res, f1(*p));
    return res;
}

template<typename It, typename F1, typename F2>
auto map_reduce_impl_async(It p, It q, F1 f1, F2 f2) -> std::future<decltype(f2(f1(*p), f1(*p)))> {
    return std::async(std::launch::async, 
                      [=] { return map_reduce_impl(p, q, f1, f2); } );
}

template<typename It, typename F1, typename F2>
auto map_reduce(It p, It q, F1 f1, F2 f2, size_t threads) -> decltype(f2(f1(*p), f1(*p))) {
    auto size = std::distance(p, q);
    auto inter = size / threads;
    auto inter_mod = size % threads;

    using Res_type = decltype(map_reduce_impl(p, q, f1, f2));
    std::vector<std::future<Res_type>> results;

    while (p != q) {
        It temp_start(p);
        auto i = inter;
        if (inter_mod) {
            ++i;
            --inter_mod;
        }
        std::advance(p, i);
        results.push_back(map_reduce_impl_async(temp_start, p, f1, f2));
    }
    
    return map_reduce_impl(results.begin(), 
                           results.end(), 
                           [](std::future<Res_type>& res) { return res.get(); },
                           f2);
}

int main() {
    std::vector<int> l = { 1,2,3,4,5,6,7,8,9,10 };
    // параллельное суммирование в 3 потока
    auto sum = map_reduce(l.begin(), l.end(),
        [](int i) { return i; },
        std::plus<int>(), 3);
    std::cout << sum << std::endl;


    // проверка наличия чётных чисел в четыре потока
    auto has_even = map_reduce(l.begin(), l.end(),
        [](int i) { return i % 2 == 0; },
        std::logical_or<bool>(), 4);
    std::cout << has_even << std::endl;

    return 0;
}

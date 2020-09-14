#include <iostream>
#include <thread>
#include <vector>

template<typename It, typename F1, typename F2>
auto map_reduce_impl(It p, It q, F1 f1, F2 f2) -> decltype(f2(f1(*p), f1(*p))) {
    auto res = f1(*p++);
    for (; p != q; ++p)
        res = f2(res, f1(*p));
    return res;
}

template<typename It, typename F1, typename F2>
auto map_reduce(It p, It q, F1 f1, F2 f2, size_t threads) -> decltype(f2(f1(*p), f1(*p))) {
    auto size = std::distance(p, q);
    auto inter = size / threads;
    auto inter_mod = size % threads;
    size_t j = 0;

    using Res_type = decltype(map_reduce_impl(p, q, f1, f2));
    std::vector<Res_type> results(threads);

    std::vector<std::thread> v_thread;

    while (p != q) {
        It temp_start(p);
        auto i = inter;
        if (inter_mod) {
            ++i;
            --inter_mod;
        }
        std::advance(p, i);

        std::thread t([=, &results]{ results[j] = map_reduce_impl(temp_start, p, f1, f2);} );
        ++j;

        v_thread.push_back(std::move(t));
    }
    
    for (std::thread& t : v_thread)
        t.join();

    return map_reduce_impl(results.begin(), 
                           results.end(), 
                           [](Res_type res) { return res; },
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

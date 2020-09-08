#include <algorithm>
#include <iostream>
#include <array>
#include <vector>

template<class Iterator>
size_t count_permutations(Iterator p, Iterator q) {
    using T = typename std::iterator_traits<Iterator>::value_type;
    std::vector<T> v(p, q);

    std::sort(v.begin(), v.end());
    size_t counter = 0;
    do {
        if (std::adjacent_find(v.begin(), v.end()) == v.end())
            ++counter;
    } while (std::next_permutation(v.begin(), v.end()));
      
    return counter;
}

int main() {
    std::array<int, 3> a1 = { 1,2,3 };
    size_t c1 = count_permutations(a1.begin(), a1.end()); // 6
    std::cout << c1 << std::endl;

    std::array<int, 5> a2 = { 1,2,3,4,4 };
    size_t c2 = count_permutations(a2.begin(), a2.end()); //36
    std::cout << c2 << std::endl;
}

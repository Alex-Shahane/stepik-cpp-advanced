#include <list>
#include <iostream>
#include <algorithm>

template<class It>
size_t max_increasing_len(It p, It q) {
    size_t max = 0;
    size_t counter = 1;
    auto left = p;
    for (; p != q; ++p) {
        if (*left < *p) {
            ++counter;
        }
        else {
            counter = 1;
        }
        max = std::max(max, counter);
        left = p;
    }
    return max;
}

int main() {
    std::list<int> const l = { 3,2,1 };
    int len1 = max_increasing_len(l.begin(), l.end());
    std::cout << len1 << "  {3, 2, 1}" << std::endl;
   
    std::list<int> const l2 = { 7,8,9,4,5,6,1,2,3,4 };
    size_t len2 = max_increasing_len(l2.begin(), l2.end()); // 4, соответствует подотрезку 1,2,3,4
    std::cout << len2 << "  {7, 8, 9, 4, 5, 6, 1, 2, 3, 4}" << std::endl;
 
    std::list<int> const l3 = { -3,-2,-1,0,0,1,2,3,4,5 };
    size_t len3 = max_increasing_len(l3.begin(), l3.end()); // 6, соответствует подотрезку 0,1,2,3,4,5
    std::cout << len3 << "  {-3, -2, -1, 0, 0, 1, 2, 3, 4, 5}"  << std::endl;

    std::list<int> const l4 = { 1,2,3 };
    int len4 = max_increasing_len(l4.begin(), l4.end());
    std::cout << len4 << "  {1, 2, 3}" << std::endl;
    //
    std::list<int> const l5 = {};
    int len5 = max_increasing_len(l5.begin(), l5.end());
    std::cout << len5  << "  {}"<< std::endl;
    //
    std::list<int> const l6 = { 1, 1, 1, 1, 1 };
    int len6 = max_increasing_len(l6.begin(), l6.end());
    std::cout << len6 << "  {1, 1, 1, 1, 1}" << std::endl;

    return 0;
};

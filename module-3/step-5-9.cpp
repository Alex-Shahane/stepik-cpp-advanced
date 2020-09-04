#include <algorithm>
#include <vector>
#include <iostream>

template<class FwdIt>
FwdIt remove_nth(FwdIt p, FwdIt q, size_t n) {
    return std::remove_if(p, q, [&n](decltype(*p) x){ return !n--; });
}

int main() {
    std::vector<int> v = { 0,1,2,3,4,5,6,7,8,9,10 };
    v.erase(remove_nth(v.begin(), v.end(), 5), v.end());
    // теперь в v = {0,1,2,3,4,6,7,8,9,10};
    for (int i : v)
        std::cout << i << " ";
    std::cout << std::endl;
    return 0;
}

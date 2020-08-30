#include <tuple>
#include <utility>

template<int I, int J, typename Tuple>
auto to_pair(Tuple tuple)->decltype(std::make_pair(std::get<I>(tuple), std::get<J>(tuple))) {
    return std::make_pair(std::get<I>(tuple), std::get<J>(tuple));
}
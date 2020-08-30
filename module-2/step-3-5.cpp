#include <cstddef>

struct Point {
    constexpr Point(double x, double y) 
        : x(x), y(y) 
    {}

    double x = 0;
    double y = 0;
};

constexpr Point operator+(const Point& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y);
}

constexpr Point operator-(const Point& a, const Point& b) {
    return Point(a.x - b.x, a.y - b.y);
}

constexpr double operator*(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y;
}

template<size_t N>
struct Array {
    int m[N];
};

int main() {
    constexpr size_t size = static_cast<size_t>(Point(2,4) * Point(4,3));
    Array<size> arr;
    return 0;
}
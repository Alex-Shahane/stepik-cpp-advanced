#include <iostream>

struct Base {
    Base(int id) : id_(id)
    { }

    int id_;
}; 

struct D1 : Base { 
    D1() : Base(1)
    { }
};

struct D2 : Base {
    D2() : Base(2)
    { }
}; 

struct D3 : D1, D2 { };

const Base* D1BaseToD2Base(const Base* base) {
    const D1* d1 = static_cast<const D1*>(base);
    const D3* d3 = static_cast<const D3*>(d1);
    const D2* d2 = static_cast<const D2*>(d3);
    return static_cast<const Base*>(d2);
}

int main() {
    D3 d3;
    const D3* dd3 = &d3;
    const D1* dd1 = static_cast<const D1*>(dd3);
    
    const Base* base = static_cast<const Base*>(dd1);
    std::cout << base->id_ << std::endl;

    const Base* new_base = D1BaseToD2Base(base);
    std::cout << new_base->id_ << std::endl;
    return 0;
}

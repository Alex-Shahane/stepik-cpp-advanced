#include <typeinfo>
#include <typeindex>
#include <functional>
#include <map>
#include <utility>

struct Shape { };

struct Rectangle : Shape { };

struct Triangle : Shape { };

bool is_intersect_r_r(Shape* a, Shape* b) {
    return false;
};

bool is_intersect_r_t(Shape* a, Shape* b) {
    return false;
};

// Base - базовый класс иерархии
// Result - тип возвращаемого значения мультиметода
// Commutative - флаг, который показывает, что
// мультиметод коммутативный (т.е. f(x,y) = f(y,x)).
template<class Base, class Result, bool Commutative>
struct Multimethod2 {

    // устанавливает реализацию мультиметода
    // для типов t1 и t2 заданных через typeid 
    // f - это функция или функциональный объект
    // принимающий два указателя на Base 
    // и возвращающий значение типа Result
    void addImpl(const std::type_info& t1, const std::type_info& t2, std::function<Result(Base*, Base*)> f) {
        auto key_pair = std::make_pair(std::type_index(t1), std::type_index(t2));
        intersect_map_[key_pair] = f;
    }

    // проверяет, есть ли реализация мультиметода
    // для типов объектов a и b
    bool hasImpl(Base* a, Base* b) const {
        auto t1 = std::type_index(typeid(*a));
        auto t2 = std::type_index(typeid(*b));

        auto key_pair = std::make_pair(t1, t2);
        auto key_reverse = std::make_pair(t2, t1);

        if (Commutative)
            return intersect_map_.find(key_pair) != intersect_map_.end()
                || intersect_map_.find(key_reverse) != intersect_map_.end();

        return intersect_map_.find(key_pair) != intersect_map_.end();
    }

    // Применяет мультиметод к объектам
    // по указателям a и b
    Result call(Base* a, Base* b) const {
        auto t1 = std::type_index(typeid(*a));
        auto t2 = std::type_index(typeid(*b));

        auto key_pair = std::make_pair(t1, t2);
        auto key_reverse = std::make_pair(t2, t1);

        auto it = intersect_map_.find(key_pair);
        auto it_rev = intersect_map_.find(key_reverse);
        
        if (it != intersect_map_.end())
            return it->second(a, b);

        if (Commutative && it_rev != intersect_map_.end())
            return it_rev->second(b, a);
    }

    std::map<std::pair<std::type_index, std::type_index>, std::function<Result(Base*, Base*)>> intersect_map_;
};

int main() {
    // мультиметод для наследников Shape
    // возращающий bool и являющийся коммутативным 
    Multimethod2<Shape, bool, true> is_intersect;

    // добавляем реализацию мультиметода для двух прямоугольников
    is_intersect.addImpl(typeid(Rectangle), typeid(Rectangle), is_intersect_r_r);

    // добавляем реализацию мультиметода для прямоугольника и треугольника
    is_intersect.addImpl(typeid(Rectangle), typeid(Triangle), is_intersect_r_t);

    // создаём две фигуры    
    Shape* s1 = new Triangle();
    Shape* s2 = new Rectangle();

    // проверяем, что реализация для s1 и s2 есть
    if (is_intersect.hasImpl(s1, s2)) {
        // вызывается функция is_intersect_r_t(s2, s1)
        bool res = is_intersect.call(s1, s2);

        // Замечание: is_intersect_r_t ожидает,
        // что первым аргументом будет прямоугольник
        // а вторым треугольник, а здесь аргументы
        // передаются в обратном порядке. 
        // ваша реализация должна самостоятельно 
        // об этом позаботиться
    }

    delete s1;
    delete s2;
}

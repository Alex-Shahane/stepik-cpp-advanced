#include <cstddef>
#include <string>

struct Unit {
    explicit Unit(size_t id) 
        : id_(id) 
    {}

    size_t id() const { return id_; }

private:
    size_t id_;
};

struct Animal: virtual Unit {
    // name хранит название животного
    // "bear" для медведя
    // "pig" для свиньи
    Animal(std::string const & name, size_t id)
        : Unit(id)
        , name_(name) 
    { }

    std::string const& name() const { return name_; }

private:
    std::string name_;
};

struct Man: virtual Unit {
    explicit Man(size_t id)
        : Unit(id)
    {}
};

struct Bear: Animal {
    explicit Bear(size_t id)
        : Animal("bear", id)
        , Unit(id)
    { }

};

// класс для свиньи
struct Pig: Animal {
    explicit Pig(size_t id)
        : Animal("pig", id)
        , Unit(id)
    { }
};

// класс для челмедведосвина
struct ManBearPig:  Man, Bear, Pig {
    ManBearPig(size_t id)
        : Unit(id)
        , Pig(id)
        , Bear(id)
        , Man(id)
    {}
};

#include <string>
#include <sstream>
#include <exception>
#include <iostream>

// описание класса исключения bad_from_string
class bad_from_string : public std::exception {
public:
    bad_from_string(const std::string& error)
        : m_error(error) 
    {}

    const char* what() const noexcept {
        return m_error.c_str();
    }

private:
    std::string m_error;
};

// функция from_string
template<class T>
T from_string(const std::string& s) {
    std::istringstream inp(s);
    
    T value;
    inp >> std::noskipws >> value;
    
    if (inp.fail() || inp.peek() != std::char_traits<char>::eof())
        throw bad_from_string(s);

    return value;
}

int main() {
    std::string s1("123");
    int    a1 = from_string<int>   (s1); // a1 = 123
    double b1 = from_string<double>(s1); // b1 = 123.0
    std::string c1 = from_string<std::string>(s1); // c1 = "123"

    std::string s2("12.3");
    int    a2 = from_string<int>   (s2); // исключение
    double b2 = from_string<double>(s2); // b2 = 12.3
    std::string c2 = from_string<std::string>(s2); // c2 = "12.3"

    std::string s3("abc");
    int    a3 = from_string<int>   (s3); // исключение
    double b3 = from_string<double>(s3); // исключение
    std::string c3 = from_string<std::string>(s3); // c3 = "abc"
    
    return 0;
}

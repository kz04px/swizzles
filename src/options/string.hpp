#ifndef OPTIONS_STRING_HPP
#define OPTIONS_STRING_HPP

#include <ostream>
#include <string>

namespace options {

struct String {
    std::string name;
    std::string value;
};

inline std::ostream &operator<<(std::ostream &os, const String &string) {
    os << "option";
    os << " name " << string.name;
    os << " type string";
    os << " default " << string.value;
    return os;
}

}  // namespace options

#endif

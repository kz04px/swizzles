#ifndef SWIZZLES_SETTINGS_HPP
#define SWIZZLES_SETTINGS_HPP

#include <string>
#include <vector>

namespace swizzles::settings {

struct Check {
    std::string name;
    bool value = false;
};

struct Spin {
    std::string name;
    int min = 0;
    int max = 0;
    int val = 0;
};

struct Button {
    std::string name;
};

struct Combo {
    std::string name;
    std::vector<std::string> values;
};

struct String {
    std::string name;
    std::string value;
};

extern Spin hash;
extern Spin threads;

}  // namespace swizzles::settings

#endif

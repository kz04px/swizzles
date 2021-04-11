#include "uci.hpp"

namespace uci {

template <typename T>
[[nodiscard]] constexpr T clamp(const T min, const T max, const T val) {
    return (val < min ? min : val > max ? max : val);
}

static_assert(clamp(1, 3, 5) == 3);
static_assert(clamp(1, 3, 2) == 2);
static_assert(clamp(1, 3, -2) == 1);
static_assert(clamp(1, 3, 1) == 1);
static_assert(clamp(1, 3, 3) == 3);

void setoption(std::stringstream &ss) {
    std::string word;

    ss >> word;
    if (word != "name") {
        return;
    }

    // Collect option name
    std::string name = "";
    while (ss >> word && word != "value") {
        if (name != "") {
            name += " ";
        }
        name += word;
    }

    // Collect option value
    std::string value = "";
    while (ss >> word) {
        if (value != "") {
            value += " ";
        }
        value += word;
    }
}

}  // namespace uci

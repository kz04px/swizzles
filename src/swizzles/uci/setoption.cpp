#include <iostream>
#include "uci.hpp"

namespace swizzles::uci {

template <typename T>
[[nodiscard]] constexpr auto clamp(const T low, const T high, const T val) noexcept -> T {
    if (val < low) {
        return low;
    } else if (val > high) {
        return high;
    } else {
        return val;
    }
}

static_assert(clamp(1, 3, 0) == 1);
static_assert(clamp(1, 3, 1) == 1);
static_assert(clamp(1, 3, 2) == 2);
static_assert(clamp(1, 3, 3) == 3);
static_assert(clamp(1, 3, 4) == 3);

auto setoption(std::stringstream &ss, UCIState &state) noexcept -> void {
    std::string name;
    std::string value;

    ss >> name;
    if (name != "name") {
        return;
    }
    ss >> name;

    ss >> value;
    if (value != "value") {
        return;
    }
    ss >> value;

    if (name == "Hash") {
        state.hash.val = clamp(state.hash.min, state.hash.max, std::stoi(value));
    } else if (name == "Threads") {
        state.threads.val = clamp(state.threads.min, state.threads.max, std::stoi(value));
    } else if (name == "UCI_Chess960") {
    }
}

}  // namespace swizzles::uci

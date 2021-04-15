#ifndef OPTIONS_COMBO_HPP
#define OPTIONS_COMBO_HPP

#include <ostream>
#include <string>
#include <vector>

namespace options {

struct Combo {
    std::string name;
    std::size_t selected;
    std::vector<std::string> choices;

    const auto set(const std::string &option) noexcept {
        for (std::size_t i = 0; i < choices.size(); ++i) {
            if (choices[i] == option) {
                selected = i;
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] const auto &get() const {
        return choices.at(selected);
    }
};

inline std::ostream &operator<<(std::ostream &os, const Combo &combo) {
    os << "option";
    os << " name " << combo.name;
    os << " type combo";
    os << " default " << combo.choices.at(combo.selected);
    for (const auto &name : combo.choices) {
        os << " var " << name;
    }
    return os;
}

}  // namespace options

#endif

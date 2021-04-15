#ifndef OPTIONS_SPIN_HPP
#define OPTIONS_SPIN_HPP

#include <ostream>
#include <string>
#include "../../util.hpp"

namespace options {

class Spin {
    using value_type = int;

   public:
    Spin(const std::string &name, const value_type min, const value_type max, const value_type value)
        : m_name{name}, m_value{}, m_min{min}, m_max{max} {
        set(value);
    }

    void set(const value_type n) noexcept {
        m_value = util::clamp(m_min, m_max, n);
    }

    [[nodiscard]] std::string name() const noexcept {
        return m_name;
    }

    [[nodiscard]] value_type value() const noexcept {
        return m_value;
    }

    [[nodiscard]] value_type min() const noexcept {
        return m_min;
    }

    [[nodiscard]] value_type max() const noexcept {
        return m_max;
    }

   private:
    std::string m_name;
    value_type m_value;
    value_type m_min;
    value_type m_max;
};

inline std::ostream &operator<<(std::ostream &os, const Spin &spin) {
    os << "option";
    os << " name " << spin.name();
    os << " type spin";
    os << " default " << spin.value();
    os << " min " << spin.min();
    os << " max " << spin.max();
    return os;
}

}  // namespace options

#endif

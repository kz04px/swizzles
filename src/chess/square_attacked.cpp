#include "movegen.hpp"
#include "position.hpp"

namespace chess {

[[nodiscard]] bool Position::square_attacked(const Square sq, const Side s) const noexcept {
    return !attackers(sq, s).empty();
}

}  // namespace chess

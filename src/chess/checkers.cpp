#include "movegen.hpp"
#include "position.hpp"

namespace chess {

[[nodiscard]] Bitboard Position::checkers() const noexcept {
    return attackers(king_position(turn()), !turn());
}

}  // namespace chess

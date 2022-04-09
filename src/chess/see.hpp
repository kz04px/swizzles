#ifndef CHESS_SEE_HPP
#define CHESS_SEE_HPP

#include <array>

namespace chess {

class Position;
class Move;

[[nodiscard]] auto see_move(const Position &pos, const Move &move, const std::array<int, 7> &values) noexcept -> int;

}  // namespace chess

#endif

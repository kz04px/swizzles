#ifndef SEARCH_IS_ENDGAME_HPP
#define SEARCH_IS_ENDGAME_HPP

#include "../chess/position.hpp"

namespace search {

[[nodiscard]] inline bool is_endgame(const chess::Position &pos) {
    const auto piece_mask = pos.piece(chess::Piece::Knight) | pos.piece(chess::Piece::Bishop) |
                            pos.piece(chess::Piece::Rook) | pos.piece(chess::Piece::Queen);
    return (piece_mask & pos.occupancy(pos.turn())).count() <= 2;
}

}  // namespace search

#endif

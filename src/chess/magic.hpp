#ifndef CHESS_MAGIC_HPP
#define CHESS_MAGIC_HPP

#include "bitboard.hpp"
#include "square.hpp"

namespace chess::magic {

[[nodiscard]] auto bishop_moves(const Square sq, const Bitboard occ) noexcept -> Bitboard;
[[nodiscard]] auto rook_moves(const Square sq, const Bitboard occ) noexcept -> Bitboard;
[[nodiscard]] auto queen_moves(const Square sq, const Bitboard occ) noexcept -> Bitboard;

}  // namespace chess::magic

#endif

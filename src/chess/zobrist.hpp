#ifndef CHESS_ZOBRIST_HPP
#define CHESS_ZOBRIST_HPP

#include <cstdint>
#include "colour.hpp"
#include "piece.hpp"
#include "square.hpp"

namespace chess::zobrist {

using hash_type = std::uint64_t;

[[nodiscard]] auto turn_key() noexcept -> hash_type;

[[nodiscard]] auto castling_key(const int t) noexcept -> hash_type;

[[nodiscard]] auto piece_key(const PieceType p, const Colour c, const Square sq) noexcept -> hash_type;

[[nodiscard]] auto ep_key(const Square sq) noexcept -> hash_type;

}  // namespace chess::zobrist

#endif

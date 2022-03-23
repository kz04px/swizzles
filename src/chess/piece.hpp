#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP

namespace chess {

enum class PieceType : int
{
    Pawn = 0,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    None,
};

[[nodiscard]] constexpr inline auto index(const PieceType piece) noexcept -> int {
    return static_cast<int>(piece);
}

}  // namespace chess

#endif

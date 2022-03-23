#include "magic.hpp"
#include "position.hpp"

namespace chess {

[[nodiscard]] auto Position::is_attacked(const Square sq, const Colour side) const noexcept -> bool {
    const auto bb = Bitboard(sq);
    auto attacks = Bitboard();

    if (side == Colour::White) {
        attacks |= get_pawns(Colour::White) & (bb.south().east() | bb.south().west());
    } else {
        attacks |= get_pawns(Colour::Black) & (bb.north().east() | bb.north().west());
    }

    attacks |= bb.knight() & get_knights(side);

    attacks |= magic::bishop_moves(sq, get_occupied()) & (get_bishops(side) | get_queens(side));

    attacks |= magic::rook_moves(sq, get_occupied()) & (get_rooks(side) | get_queens(side));

    attacks |= bb.adjacent() & get_kings(side);

    return !attacks.empty();
}

[[nodiscard]] auto Position::is_attacked(const Bitboard bb, const Colour side) const noexcept -> bool {
    // Pawns
    if (side == Colour::White) {
        const auto attacks = get_pawns(Colour::White).north().east() | get_pawns(Colour::White).north().west();
        if (attacks & bb) {
            return true;
        }
    } else {
        const auto attacks = get_pawns(Colour::Black).south().east() | get_pawns(Colour::Black).south().west();
        if (attacks & bb) {
            return true;
        }
    }

    // Knights
    if (bb & get_knights(side).knight()) {
        return true;
    }

    // King
    if (bb.adjacent() & get_kings(side)) {
        return true;
    }

    for (const auto sq : bb) {
        // Bishops & Queens
        if (magic::bishop_moves(sq, get_occupied()) & (get_bishops(side) | get_queens(side))) {
            return true;
        }

        // Rooks & Queens
        if (magic::rook_moves(sq, get_occupied()) & (get_rooks(side) | get_queens(side))) {
            return true;
        }
    }

    return false;
}

}  // namespace chess

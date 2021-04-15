#ifndef EVAL_KING_SAFETY_HPP
#define EVAL_KING_SAFETY_HPP

#include "../chess/position.hpp"
#include "../score.hpp"

namespace eval {

template <chess::Side side>
[[nodiscard]] constexpr chess::Bitboard shield(const chess::Bitboard bb) {
    chess::Bitboard mask = bb.forwards<side>();
    mask |= mask.forwards<side>();
    mask |= mask.east();
    mask |= mask.west();
    return mask;
}

template <chess::Side side>
[[nodiscard]] constexpr chess::Bitboard shield(const chess::Square sq) {
    return shield<side>(chess::Bitboard(sq));
}

template <chess::Side side>
[[nodiscard]] Score king_safety(const chess::Position &pos) {
    /*
    const auto bb = pos.kings<side>();
    const auto pawns = shield<side>(bb) & pos.pawns<side>();
    return Score{15, 0} * pawns.count();
    */

    const auto bb = pos.kings<side>();
    const auto surrounding = bb.adjacent();

    // Positive: Nearby friendly pieces
    return Score{5, 5} * (surrounding & pos.colour<side>()).count();
}

static_assert(shield<chess::Side::White>(chess::squares::G1) == chess::Bitboard(0xe0e000));
static_assert(shield<chess::Side::Black>(chess::squares::G8) == chess::Bitboard(0xe0e00000000000));

}  // namespace eval

#endif

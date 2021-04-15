#ifndef EVAL_MOBILITY_HPP
#define EVAL_MOBILITY_HPP

#include "../chess/movegen.hpp"
#include "../chess/position.hpp"
#include "../score.hpp"

namespace eval {

template <chess::Side side>
[[nodiscard]] Score mobility(const chess::Position &pos) {
    auto moves = chess::Bitboard();

    // Knights
    for (const auto sq : pos.knights<side>()) {
        moves |= chess::movegen::knight_moves(sq);
    }

    // Bishops
    for (const auto sq : pos.bishops<side>()) {
        moves |= chess::movegen::bishop_moves(sq, pos.occupied());
    }

    // Rooks
    for (const auto sq : pos.rooks<side>()) {
        moves |= chess::movegen::rook_moves(sq, pos.occupied());
    }

    // Queens
    for (const auto sq : pos.queens<side>()) {
        moves |= chess::movegen::queen_moves(sq, pos.occupied());
    }

    // Ignore moves that land on our own pieces
    moves &= ~pos.colour<side>();

    return Score{4, 4} * moves.count();
}

}  // namespace eval

#endif

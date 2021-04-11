#include "eval.hpp"
#include "../score.hpp"
#include "phase.hpp"

namespace eval {

constexpr Score piece_values[] = {
    {100, 100},
    {320, 320},
    {330, 330},
    {500, 500},
    {900, 900},
    {1000000, 1000000},
};

template <chess::Side side>
[[nodiscard]] Score eval_side(const chess::Position &pos) {
    Score score;

    for (const auto p : chess::pieces) {
        // Piece values
        score += piece_values[static_cast<int>(p)] * pos.pieces(side, p).count();
    }

    return score;
}

[[nodiscard]] int eval(const chess::Position &pos) {
    Score score;
    score += eval_side<chess::Side::White>(pos);
    score -= eval_side<chess::Side::Black>(pos);
    const auto flipped = pos.turn() == chess::Side::White ? score : -score;
    return phase(pos, flipped);
}

}  // namespace eval

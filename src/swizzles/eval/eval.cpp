#include "eval.hpp"
#include <chess/position.hpp>
#include "pst.hpp"

namespace swizzles::eval {

static constexpr std::array<Score, 6> material = {{
    {100, 100},
    {300, 300},
    {300, 300},
    {500, 500},
    {900, 900},
    {0, 0},
}};

template <chess::Colour us>
[[nodiscard]] auto eval_us(const chess::Position &pos) noexcept -> Score {
    Score score;

    // Material
    score += material[0] * pos.get_pawns(us).count();
    score += material[1] * pos.get_knights(us).count();
    score += material[2] * pos.get_bishops(us).count();
    score += material[3] * pos.get_rooks(us).count();
    score += material[4] * pos.get_queens(us).count();

    // PST
    for (const auto sq : pos.get_pawns(us)) {
        score += pst_value<us>(chess::PieceType::Pawn, sq);
    }
    for (const auto sq : pos.get_knights(us)) {
        score += pst_value<us>(chess::PieceType::Knight, sq);
    }
    for (const auto sq : pos.get_bishops(us)) {
        score += pst_value<us>(chess::PieceType::Bishop, sq);
    }
    for (const auto sq : pos.get_rooks(us)) {
        score += pst_value<us>(chess::PieceType::Rook, sq);
    }
    for (const auto sq : pos.get_queens(us)) {
        score += pst_value<us>(chess::PieceType::Queen, sq);
    }
    for (const auto sq : pos.get_kings(us)) {
        score += pst_value<us>(chess::PieceType::King, sq);
    }

    return score;
}

[[nodiscard]] auto phase(const chess::Position &pos, const Score &score) noexcept -> int {
    const auto num_knights = pos.get_knights().count();
    const auto num_bishops = pos.get_bishops().count();
    const auto num_rooks = pos.get_rooks().count();
    const auto num_queens = pos.get_queens().count();
    const auto total_phase = 24;
    auto phase = total_phase - num_knights - num_bishops - 2 * num_rooks - 4 * num_queens;
    phase = (phase * 256 + (total_phase / 2)) / total_phase;
    return ((score.mg() * (256 - phase)) + (score.eg() * phase)) / 256;
}

[[nodiscard]] auto eval(const chess::Position &pos) noexcept -> int {
    Score score;
    score += eval_us<chess::Colour::White>(pos);
    score -= eval_us<chess::Colour::Black>(pos);
    const int phased = phase(pos, score);

    if (pos.turn() == chess::Colour::White) {
        return phased;
    } else {
        return -phased;
    }
}

}  // namespace swizzles::eval

#include "eval.hpp"
#include <chess/position.hpp>
#include "pst.hpp"

namespace swizzles::eval {

static constexpr std::array<int, 6> material = {100, 300, 300, 500, 900, 0};

template <chess::Colour us>
[[nodiscard]] auto eval_us(const chess::Position &pos) noexcept -> int {
    int score = 0;

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

[[nodiscard]] auto eval(const chess::Position &pos) noexcept -> int {
    int score = 0;

    score += eval_us<chess::Colour::White>(pos);
    score -= eval_us<chess::Colour::Black>(pos);

    if (pos.turn() == chess::Colour::White) {
        return score;
    } else {
        return -score;
    }
}

}  // namespace swizzles::eval

#include "eval.hpp"
#include <chess/position.hpp>

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

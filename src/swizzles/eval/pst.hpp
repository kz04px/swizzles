#ifndef SWIZZLES_EVAL_PST_HPP
#define SWIZZLES_EVAL_PST_HPP

#include <array>
#include <chess/colour.hpp>
#include <chess/piece.hpp>
#include <chess/square.hpp>
#include "../score.hpp"

namespace swizzles::eval {

extern const std::array<Score, 6 * 64> pst;

template <chess::Colour colour>
[[nodiscard]] auto pst_value(const chess::PieceType piece, const chess::Square sq) noexcept -> Score;

}  // namespace swizzles::eval

#endif

#ifndef EVAL_PHASE_HPP
#define EVAL_PHASE_HPP

#include "../chess/position.hpp"
#include "../score.hpp"

namespace eval {

[[nodiscard]] int phase(const chess::Position &pos, const Score &score) noexcept;

}  // namespace eval

#endif

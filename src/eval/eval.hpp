#ifndef EVAL_HPP
#define EVAL_HPP

#include "../chess/position.hpp"

namespace eval {

[[nodiscard]] int eval(const chess::Position &pos);

}  // namespace eval

#endif

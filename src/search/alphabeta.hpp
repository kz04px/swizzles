#ifndef SEARCH_ALPHABETA_HPP
#define SEARCH_ALPHABETA_HPP

#include <limits>
#include "../assert.hpp"
#include "../chess/position.hpp"
#include "constants.hpp"
#include "stack.hpp"

namespace search {

[[nodiscard]] constexpr int eval_to_tt(const int eval, const int ply) {
    UCI_ASSERT(std::abs(eval + constants::max_depth) <= std::numeric_limits<std::int16_t>::max());

    if (eval > constants::max_eval) {
        return eval + ply;
    }
    if (eval < constants::min_eval) {
        return eval - ply;
    }
    return eval;
}

[[nodiscard]] constexpr int eval_from_tt(const int eval, const int ply) {
    if (eval > constants::max_eval) {
        return eval - ply;
    }
    if (eval < constants::min_eval) {
        return eval + ply;
    }
    return eval;
}

int alphabeta(chess::Position &pos, Stack *ss, const int alpha, int beta, const int depth);

}  // namespace search

#endif

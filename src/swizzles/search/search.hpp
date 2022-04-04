#ifndef SWIZZLES_SEARCH_HPP
#define SWIZZLES_SEARCH_HPP

#include "constants.hpp"
#include "stack.hpp"
#include "thread_data.hpp"

namespace chess {
class Position;
}  // namespace chess

namespace swizzles::search {

[[nodiscard]] constexpr auto eval_to_tt(const int eval, const int ply) noexcept -> int {
    if (eval > mate_score - max_depth) {
        return eval + ply;
    }
    if (eval < -mate_score + max_depth) {
        return eval - ply;
    }
    return eval;
}

[[nodiscard]] constexpr auto eval_from_tt(const int eval, const int ply) noexcept -> int {
    if (eval > mate_score - max_depth) {
        return eval - ply;
    }
    if (eval < -mate_score + max_depth) {
        return eval + ply;
    }
    return eval;
}

auto reduction(const chess::Move move,
               const int depth,
               const int legal_moves,
               const bool in_check,
               const bool gives_check,
               const bool is_root);

[[nodiscard]] auto search(ThreadData &td,
                          SearchStack *ss,
                          chess::Position &pos,
                          int alpha,
                          int beta,
                          int depth) noexcept -> int;

}  // namespace swizzles::search

#endif

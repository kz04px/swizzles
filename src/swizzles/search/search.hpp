#ifndef SWIZZLES_SEARCH_HPP
#define SWIZZLES_SEARCH_HPP

#include "stack.hpp"
#include "thread_data.hpp"

namespace chess {
class Position;
}  // namespace chess

namespace swizzles::search {

static constexpr int inf_score = 2'000'000;
static constexpr int mate_score = 1'000'000;

[[nodiscard]] auto search(ThreadData &td,
                          SearchStack *ss,
                          chess::Position &pos,
                          int alpha,
                          const int beta,
                          int depth) noexcept -> int;

}  // namespace swizzles::search

#endif

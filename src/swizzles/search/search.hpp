#ifndef SWIZZLES_SEARCH_HPP
#define SWIZZLES_SEARCH_HPP

#include "stack.hpp"
#include "thread_data.hpp"

namespace chess {
class Position;
}  // namespace chess

namespace swizzles::search {

[[nodiscard]] auto search(ThreadData &td, SearchStack *ss, chess::Position &pos, const int depth) noexcept -> int;

}  // namespace swizzles::search

#endif

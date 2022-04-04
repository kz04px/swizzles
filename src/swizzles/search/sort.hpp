#ifndef SWIZZLES_SEARCH_SORT_HPP
#define SWIZZLES_SEARCH_SORT_HPP

#include <chess/move.hpp>
#include <chess/movelist.hpp>
#include "search.hpp"

namespace swizzles::search {

auto sort(chess::MoveList &movelist, const chess::Move ttmove, const ThreadData &td, const chess::Colour turn) noexcept
    -> void;

}  // namespace swizzles::search

#endif

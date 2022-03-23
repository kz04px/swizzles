#ifndef SWIZZLES_SEARCH_SORT_HPP
#define SWIZZLES_SEARCH_SORT_HPP

#include <chess/movelist.hpp>

namespace swizzles::search {

auto sort(chess::MoveList &movelist) noexcept -> void;

}  // namespace swizzles::search

#endif

#ifndef SWIZZLES_SEARCH_ROOT_HPP
#define SWIZZLES_SEARCH_ROOT_HPP

#include <atomic>
#include "../uci/state.hpp"
#include "settings.hpp"

namespace swizzles::search {

[[nodiscard]] auto root(const uci::UCIState &state, const SearchSettings settings, std::atomic<bool> &stop) noexcept
    -> Results;

}  // namespace swizzles::search

#endif

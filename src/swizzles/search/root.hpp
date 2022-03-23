#ifndef SWIZZLES_SEARCH_ROOT_HPP
#define SWIZZLES_SEARCH_ROOT_HPP

#include <atomic>
#include "settings.hpp"

namespace swizzles::search {

[[nodiscard]] auto root(const SearchSettings settings, std::atomic<bool> &stop) noexcept -> Results;

}  // namespace swizzles::search

#endif

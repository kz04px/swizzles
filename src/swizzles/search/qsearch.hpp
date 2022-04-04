#ifndef SWIZZLES_QSEARCH_HPP
#define SWIZZLES_QSEARCH_HPP

namespace chess {
class Position;
}  // namespace chess

namespace swizzles::search {

[[nodiscard]] auto qsearch(const ThreadData &td, chess::Position &pos, int alpha, const int beta) noexcept -> int;

}  // namespace swizzles::search

#endif

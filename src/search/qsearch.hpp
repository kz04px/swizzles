#ifndef SEARCH_QSEARCH_HPP
#define SEARCH_QSEARCH_HPP

#include "../chess/position.hpp"

namespace search {

[[nodiscard]] int qsearch(chess::Position &pos, int alpha, const int beta);

}  // namespace search

#endif

#ifndef SEARCH_ALPHABETA_HPP
#define SEARCH_ALPHABETA_HPP

#include "../chess/position.hpp"
#include "stack.hpp"

namespace search {

int alphabeta(chess::Position &pos, Stack *ss, const int alpha, const int beta, const int depth);

}  // namespace search

#endif

#ifndef SEARCH_SEE_HPP
#define SEARCH_SEE_HPP

#include "../chess/move.hpp"
#include "../chess/position.hpp"
#include "../chess/square.hpp"

namespace search {

[[nodiscard]] int SEEcapture(chess::Position &pos, const chess::Move &move);
[[nodiscard]] int SEEquiet(chess::Position &pos, const chess::Move &move);

}  // namespace search

#endif

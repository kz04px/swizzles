#ifndef SEARCH_SEARCH_HPP
#define SEARCH_SEARCH_HPP

#include <cstdint>
#include <functional>
#include "../chess/move.hpp"
#include "../chess/position.hpp"
#include "pv.hpp"

namespace search {

using info_printer_type = std::function<void(int depth, int score, std::uint64_t nodes, int time, const PV &pv)>;

std::pair<chess::Move, chess::Move> search(chess::Position pos);
std::pair<chess::Move, chess::Move> search(chess::Position pos, const info_printer_type &info_printer);

}  // namespace search

#endif

#ifndef SEARCH_PV_HPP
#define SEARCH_PV_HPP

#include <vector>
#include "../chess/position.hpp"

using PV = std::vector<chess::Move>;

[[nodiscard]] bool legal_pv(chess::Position &pos, const PV &pv);

#endif

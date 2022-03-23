#ifndef SWIZZLES_SEARCH_RESULTS_HPP
#define SWIZZLES_SEARCH_RESULTS_HPP

#include <chess/move.hpp>
#include <cstdint>

namespace swizzles::search {

struct Results {
    chess::Move bestmove;
    chess::Move ponder;
    std::uint64_t nodes = 0;
    int depth = 0;
    int seldepth = 0;
    int eval = 0;
    std::uint64_t ms = 0;
    int tbhits = 0;
};

}  // namespace swizzles::search

#endif

#ifndef SWIZZLES_SEARCH_SETTINGS_HPP
#define SWIZZLES_SEARCH_SETTINGS_HPP

#include <chess/position.hpp>
#include <cstdint>
#include <functional>
#include "pv.hpp"
#include "results.hpp"

namespace swizzles::search {

enum class SearchType : int
{
    Time = 0,
    Depth,
    Movetime,
    Nodes,
    Infinite,
};

using info_printer_type = std::function<void(const int depth,
                                             const int seldepth,
                                             const int eval,
                                             const std::uint64_t ms,
                                             const std::uint64_t nodes,
                                             const std::uint64_t nps,
                                             const int tbhits,
                                             const int hashfull,
                                             const PV &pv)>;

struct SearchSettings {
    SearchType type = SearchType::Depth;
    int wtime = 0;
    int btime = 0;
    int winc = 0;
    int binc = 0;
    int movestogo = 0;
    int depth = 1;
    int movetime = 0;
    info_printer_type info_printer = [](const int,
                                        const int,
                                        const int,
                                        const std::uint64_t,
                                        const std::uint64_t,
                                        const std::uint64_t,
                                        const int,
                                        const int,
                                        const swizzles::search::PV &) {
    };
};

}  // namespace swizzles::search

#endif

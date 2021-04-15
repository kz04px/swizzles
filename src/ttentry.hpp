#ifndef TTENTRY_HPP
#define TTENTRY_HPP

#include <cstdint>
#include "chess/move.hpp"

enum class Flag : std::uint8_t
{
    Exact,
    Upper,
    Lower,
};

struct TTEntry {
    std::uint64_t hash;
    chess::Move move;
    std::int16_t score;
    std::uint8_t depth;
    Flag flag;
};

static_assert(sizeof(TTEntry) == 2 * sizeof(std::uint64_t));

#endif

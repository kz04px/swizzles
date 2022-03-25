#ifndef SWIZZLES_TTENTRY_HPP
#define SWIZZLES_TTENTRY_HPP

#include <chess/move.hpp>
#include <chess/zobrist.hpp>

namespace swizzles {

enum class TTFlag
{
    Upper = 0,
    Lower,
    Exact,
};

struct TTEntry {
    chess::zobrist::hash_type hash = 0;
    chess::Move move;
    int eval = 0;
    int depth = 0;
    TTFlag flag = TTFlag::Exact;
};

}  // namespace swizzles

#endif

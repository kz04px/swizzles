#ifndef SEARCH_STACK_HPP
#define SEARCH_STACK_HPP

#include "../chess/move.hpp"
#include "pv.hpp"

namespace search {

struct Stack {
    int ply;
    PV pv;
    chess::Move killer_1;
    bool nullmove;
};

}  // namespace search

#endif

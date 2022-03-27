#ifndef SWIZZLES_SEARCH_STACK_HPP
#define SWIZZLES_SEARCH_STACK_HPP

#include "pv.hpp"

namespace swizzles::search {

struct SearchStack {
    int ply = 0;
    bool null_move = false;
    PV pv;
};

}  // namespace swizzles::search

#endif

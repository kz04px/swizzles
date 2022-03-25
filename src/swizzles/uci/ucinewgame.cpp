#include <chess/position.hpp>
#include <iostream>
#include <sstream>
#include "uci.hpp"

namespace swizzles::uci {

auto ucinewgame(UCIState &state) noexcept -> void {
    state.pos.set_fen("startpos");
    state.tt->clear();
}

}  // namespace swizzles::uci

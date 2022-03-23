#include <chess/position.hpp>
#include <iostream>
#include <sstream>
#include "uci.hpp"

namespace swizzles::uci {

auto ucinewgame(chess::Position &pos) noexcept -> void {
    pos.set_fen("startpos");
}

}  // namespace swizzles::uci

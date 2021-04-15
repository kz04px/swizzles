#include "../chess/position.hpp"
#include "../globals.hpp"
#include "uci.hpp"

namespace uci {

void ucinewgame(chess::Position &pos) {
    pos.set_fen("startpos");
    tt.clear();
}

}  // namespace uci

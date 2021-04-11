#include "../chess/position.hpp"
#include "uci.hpp"

namespace uci {

void ucinewgame(chess::Position &pos) {
    pos.set_fen("startpos");
}

}  // namespace uci

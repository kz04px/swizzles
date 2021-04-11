#include "../chess/position.hpp"
#include "uci.hpp"

namespace uci {

void moves(chess::Position &pos, std::stringstream &ss) {
    std::string word;
    while (ss >> word) {
        const auto legal_moves = pos.legal_moves();
        bool found = false;
        for (const auto &move : legal_moves) {
            if (word == static_cast<std::string>(move)) {
                found = true;
                pos.makemove(move);
                break;
            }
        }
        if (!found) {
            break;
        }
    }
}

}  // namespace uci

#include "../chess/position.hpp"
#include "uci.hpp"

namespace uci {

void position(chess::Position &pos, std::stringstream &ss) {
    std::string word;
    std::string fen;
    ss >> word;

    // Position
    if (word == "startpos") {
        fen = "startpos";
        ss >> word;
    } else if (word == "fen") {
        // Collect fen string
        while (ss >> word && word != "moves") {
            if (fen != "") {
                fen += " ";
            }
            fen += word;
        }
    } else {
        return;
    }

    pos.set_fen(fen);

    if (word == "moves") {
        moves(pos, ss);
    }
}

}  // namespace uci

#include "uci.hpp"

namespace swizzles::uci {

auto position(std::stringstream &ss, chess::Position &pos) noexcept -> void {
    std::string fen;

    while (!ss.eof()) {
        std::string word;
        ss >> word;

        if (word == "moves") {
            pos.set_fen(fen);
            moves(ss, pos);
            return;
        } else if (word == "fen") {
        } else if (fen.empty()) {
            fen = word;
        } else {
            fen += " " + word;
        }
    }

    pos.set_fen(fen);
}

}  // namespace swizzles::uci

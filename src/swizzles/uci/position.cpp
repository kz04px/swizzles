#include "uci.hpp"

namespace swizzles::uci {

auto position(std::stringstream &ss, UCIState &state) noexcept -> void {
    std::string fen;

    while (!ss.eof()) {
        std::string word;
        ss >> word;

        if (word == "moves") {
            state.pos.set_fen(fen);
            moves(ss, state);
            return;
        } else if (word == "fen") {
        } else if (fen.empty()) {
            fen = word;
        } else {
            fen += " " + word;
        }
    }

    state.pos.set_fen(fen);
}

}  // namespace swizzles::uci

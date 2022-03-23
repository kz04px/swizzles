#include <chess/position.hpp>
#include <iostream>
#include <sstream>
#include "uci.hpp"

namespace swizzles::uci {

auto parse(std::stringstream &ss, chess::Position &pos) noexcept -> bool {
    while (!ss.eof()) {
        std::string word;
        ss >> word;

        if (word == "position") {
            position(ss, pos);
        } else if (word == "ucinewgame") {
            ucinewgame(pos);
        } else if (word == "isready") {
            std::cout << "readyok" << std::endl;
        } else if (word == "go") {
            go(ss, pos);
        } else if (word == "stop") {
            stop();
        } else if (word == "quit") {
            return true;
        }
    }

    return false;
}

}  // namespace swizzles::uci

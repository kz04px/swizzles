#include <chess/position.hpp>
#include <iostream>
#include <sstream>
#include "uci.hpp"

namespace swizzles::uci {

auto parse(std::stringstream &ss, UCIState &state) noexcept -> bool {
    while (!ss.eof()) {
        std::string word;
        ss >> word;

        if (word == "position") {
            position(ss, state);
        } else if (word == "ucinewgame") {
            ucinewgame(state);
        } else if (word == "setoption") {
            setoption(ss, state);
        } else if (word == "isready") {
            std::cout << "readyok" << std::endl;
        } else if (word == "go") {
            go(ss, state);
        } else if (word == "stop") {
            stop();
        } else if (word == "quit") {
            return true;
        }
    }

    return false;
}

}  // namespace swizzles::uci

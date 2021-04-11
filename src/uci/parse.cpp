#include "extensions/extensions.hpp"
#include "uci.hpp"

namespace uci {

void parse(chess::Position &pos, const std::string &input) {
    std::stringstream ss{input};
    std::string word;
    ss >> word;

    if (word == "ucinewgame") {
        ucinewgame(pos);
    } else if (word == "isready") {
        isready();
    } else if (word == "position") {
        position(pos, ss);
    } else if (word == "moves") {
        moves(pos, ss);
    } else if (word == "go") {
        go(pos, ss);
    } else if (word == "stop") {
        stop();
    } else if (word == "setoption") {
        setoption(ss);
    } else if (word == "print" || word == "display" || word == "board") {
        extension::display(pos);
    } else if (word == "perft") {
        extension::perft(pos, ss);
    } else if (word == "split") {
        extension::split(pos, ss);
    } else if (word == "eval") {
        extension::eval(pos);
    }
}

}  // namespace uci

#include <iostream>
#include "../chess/position.hpp"
#include "../globals.hpp"
#include "extensions/extensions.hpp"
#include "uci.hpp"

namespace uci {

void listen() {
    uci();

    std::string word;
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        std::stringstream ss{input};
        ss >> word;

        if (word == "isready") {
            break;
        } else if (word == "setoption") {
            setoption(ss);
        } else if (word == "quit") {
            return;
        }
    }

    auto pos = chess::Position{"startpos"};
    tt.resize(settings::hash.value());
    isready();

    while (true) {
        std::string input;
        std::getline(std::cin, input);
        std::stringstream ss{input};
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
        } else if (word == "quit") {
            break;
        }
    }

    stop();
}

}  // namespace uci

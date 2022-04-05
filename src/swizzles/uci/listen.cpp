#include <chess/position.hpp>
#include <iostream>
#include <sstream>
#include <tt.hpp>
#include "../settings.hpp"
#include "../ttentry.hpp"
#include "state.hpp"
#include "uci.hpp"

namespace swizzles::uci {

auto operator<<(std::ostream &os, const settings::Spin &spin) noexcept -> std::ostream & {
    os << "option name " << spin.name;
    os << " type spin";
    os << " default " << spin.val;
    os << " min " << spin.min;
    os << " max " << spin.max;
    return os;
}

auto listen() noexcept -> void {
    UCIState state;

    std::cout << "id name Swizzles" << std::endl;
    std::cout << "id author kz04px" << std::endl;

    // Print options
    std::cout << "option name UCI_Chess960 type check default false\n";
    std::cout << state.hash << "\n";
    std::cout << state.threads << "\n";

    // Reply to "uci"
    std::cout << "uciok" << std::endl;

    // Wait for first "isready"
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        std::stringstream ss(input);
        ss >> input;

        if (input == "isready") {
            std::cout << "readyok" << std::endl;
            break;
        } else if (input == "setoption") {
            setoption(ss, state);
        } else if (input == "quit") {
            return;
        }
    }

    // Initialise
    state.tt = std::make_shared<TT<TTEntry>>(state.hash.val);

    bool quit = false;
    while (!quit) {
        std::string input;
        std::getline(std::cin, input);
        std::stringstream ss(input);
        quit = parse(ss, state);
    }

    stop();
}

}  // namespace swizzles::uci

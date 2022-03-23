#include <chess/position.hpp>
#include <iostream>
#include <sstream>
#include "../settings.hpp"
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
    std::cout << "engine Swizzles" << std::endl;
    std::cout << "author kz04px" << std::endl;

    // Print options
    std::cout << "option name UCI_Chess960 type check default false\n";
    std::cout << settings::hash << "\n";
    std::cout << settings::threads << "\n";

    // Reply to "uci"
    std::cout << "uciok" << std::endl;

    // Wait for first "isready"
    while (true) {
        std::string input;
        std::getline(std::cin, input);

        if (input == "isready") {
            std::cout << "readyok" << std::endl;
            break;
        } else if (input == "quit") {
            return;
        }
    }

    auto pos = chess::Position("startpos");
    bool quit = false;
    while (!quit) {
        std::string input;
        std::getline(std::cin, input);
        std::stringstream ss(input);
        quit = parse(ss, pos);
    }

    stop();
}

}  // namespace swizzles::uci

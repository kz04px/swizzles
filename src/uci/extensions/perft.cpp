#include <chrono>
#include <iostream>
#include "../../chess/position.hpp"
#include "extensions.hpp"

namespace uci::extension {

void perft(chess::Position &pos, std::stringstream &ss) {
    int depth = 0;

    if (!(ss >> depth) || depth < 0) {
        return;
    }

    const auto t0 = std::chrono::high_resolution_clock::now();
    const auto nodes = pos.perft(depth);
    const auto t1 = std::chrono::high_resolution_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

    std::cout << "info";
    std::cout << " depth " << depth;
    std::cout << " nodes " << nodes;
    std::cout << " time " << time.count();
    std::cout << std::endl;

    std::cout << "nodes " << nodes << std::endl;
}

}  // namespace uci::extension

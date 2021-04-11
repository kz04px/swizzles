#include <chrono>
#include <iostream>
#include "../../chess/position.hpp"
#include "extensions.hpp"

namespace uci::extension {

void split(chess::Position &pos, std::stringstream &ss) {
    int depth = 0;

    if (!(ss >> depth) || depth < 0) {
        return;
    }

    const auto legal_moves = pos.legal_moves();
    std::uint64_t total = 0;

    const auto t0 = std::chrono::high_resolution_clock::now();
    for (const auto &move : legal_moves) {
        pos.makemove(move);
        const auto nodes = pos.perft(depth);
        total += nodes;
        std::cout << move << " " << nodes << std::endl;
        pos.undomove();
    }
    const auto t1 = std::chrono::high_resolution_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

    std::cout << "nodes " << total;
    std::cout << " time " << time.count();
    std::cout << std::endl;
}

}  // namespace uci::extension

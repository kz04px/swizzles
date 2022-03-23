#include <chess/position.hpp>
#include <chrono>
#include <cstdint>
#include <iostream>

[[nodiscard]] auto perft(chess::Position &pos, const int depth) noexcept -> std::uint64_t {
    if (depth == 0) {
        return 1ULL;
    }

    std::uint64_t nodes = 0ULL;
    const auto moves = pos.movegen();

    for (const auto &move : moves) {
        pos.makemove<false>(move);

        if (pos.is_attacked(pos.get_king(!pos.turn()), pos.turn())) {
            pos.undomove();
            continue;
        }

        nodes += perft(pos, depth - 1);
        pos.undomove();
    }

    return nodes;
}

int main(const int argc, const char **argv) {
    int depth = 1;
    std::string fen = "startpos";

    // Get depth
    if (argc > 1) {
        depth = std::stoi(argv[1]);
    }

    // Get FEN
    for (int i = 2; i < argc; ++i) {
        if (i == 2) {
            fen = std::string(argv[i]);
        } else {
            fen += " " + std::string(argv[i]);
        }
    }

    auto pos = chess::Position(fen);
    std::cout << pos << "\n";

    for (int i = 1; i <= depth; ++i) {
        const auto t0 = std::chrono::steady_clock::now();
        const auto nodes = perft(pos, i);
        const auto t1 = std::chrono::steady_clock::now();
        const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

        std::cout << "info";
        std::cout << " depth " << i;
        std::cout << " nodes " << nodes;
        std::cout << " time " << dt.count();
        if (dt.count() > 0) {
            std::cout << " nps " << 1000 * nodes / dt.count();
        }
        std::cout << "\n";
    }

    return 0;
}

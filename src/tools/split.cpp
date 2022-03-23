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
        pos.makemove(move);

        if (pos.is_attacked(pos.get_king(!pos.turn()), pos.turn())) {
            pos.undomove();
            continue;
        }

        nodes += perft(pos, depth - 1);
        pos.undomove();
    }

    return nodes;
}

void makemove(chess::Position &pos, const std::string &movestr) {
    for (const auto move : pos.movegen()) {
        if (static_cast<std::string>(move) == movestr) {
            pos.makemove(move);
            return;
        }
    }
    throw std::invalid_argument("Couldn't find the move string: \"" + movestr + "\"");
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

    int idx = 1;
    std::uint64_t total = 0;
    const auto t0 = std::chrono::steady_clock::now();

    for (const auto move : pos.movegen()) {
        pos.makemove(move);

        if (pos.is_attacked(pos.get_king(!pos.turn()), pos.turn())) {
            pos.undomove();
            continue;
        }

        const auto nodes = perft(pos, depth - 1);
        pos.undomove();

        total += nodes;
        std::cout << idx << " " << move << ": " << nodes << "\n";
        idx++;
    }
    const auto t1 = std::chrono::steady_clock::now();
    const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

    const auto seconds = dt.count() / 1000;
    const auto milliseconds = dt.count() % 1000;
    std::cout << "\n";
    std::cout << "Nodes " << total << "\n";
    std::cout << "Time: " << seconds << "." << milliseconds << "s\n";

    return 0;
}

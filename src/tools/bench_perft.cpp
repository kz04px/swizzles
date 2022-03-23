#include <algorithm>
#include <array>
#include <chess/position.hpp>
#include <chrono>
#include <cstdint>
#include <iomanip>
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

[[nodiscard]] auto format_ms(const std::chrono::milliseconds ms) noexcept -> std::string {
    const auto seconds = ms.count() / 1000;
    const auto milliseconds = ms.count() % 1000;
    return std::to_string(seconds) + "." + std::to_string((milliseconds / 100) % 10) +
           std::to_string((milliseconds / 10) % 10) + std::to_string(milliseconds % 10);
}

int main(const int argc, const char **argv) {
    const std::array<std::string, 13> fens = {{
        "startpos",
        "bqnb1rkr/pp3ppp/3ppn2/2p5/5P2/P2P4/NPP1P1PP/BQ1BNRKR w HFhf - 2 9",
        "2nnrbkr/p1qppppp/8/1ppb4/6PP/3PP3/PPP2P2/BQNNRBKR w HEhe - 1 9",
        "b1q1rrkb/pppppppp/3nn3/8/P7/1PPP4/4PPPP/BQNNRKRB w GE - 1 9",
        "qbbnnrkr/2pp2pp/p7/1p2pp2/8/P3PP2/1PPP1KPP/QBBNNR1R w hf - 0 9",
        "1nbbnrkr/p1p1ppp1/3p4/1p3P1p/3Pq2P/8/PPP1P1P1/QNBBNRKR w HFhf - 0 9",
        "qnbnr1kr/ppp1b1pp/4p3/3p1p2/8/2NPP3/PPP1BPPP/QNB1R1KR w HEhe - 1 9",
        "q1bnrkr1/ppppp2p/2n2p2/4b1p1/2NP4/8/PPP1PPPP/QNB1RRKB w ge - 1 9",
        "qbn1brkr/ppp1p1p1/2n4p/3p1p2/P7/6PP/QPPPPP2/1BNNBRKR w HFhf - 0 9",
        "qnnbbrkr/1p2ppp1/2pp3p/p7/1P5P/2NP4/P1P1PPP1/Q1NBBRKR w HFhf - 0 9",
        "qn1rbbkr/ppp2p1p/1n1pp1p1/8/3P4/P6P/1PP1PPPK/QNNRBB1R w hd - 2 9",
        "qnr1bkrb/pppp2pp/3np3/5p2/8/P2P2P1/NPP1PP1P/QN1RBKRB w GDg - 3 9",
        "qb1nrkbr/1pppp1p1/1n3p2/p1B4p/8/3P1P1P/PPP1P1P1/QBNNRK1R w HEhe - 0 9",
    }};

    int depth = 1;
    auto total_time = std::chrono::milliseconds(0);
    std::uint64_t total_nodes = 0;

    // Get depth
    if (argc > 1) {
        depth = std::stoi(argv[1]);
    }

    // Print chart title
    std::cout << std::left;
    std::cout << std::setw(5) << "Pos";
    std::cout << std::right;
    std::cout << std::setw(10) << "Nodes";
    std::cout << std::setw(12) << "Nodes Sum";
    std::cout << std::setw(9) << "Time";
    std::cout << std::setw(10) << "Time Sum";
    std::cout << std::left;
    std::cout << "  FEN";
    std::cout << "\n";

    for (std::size_t i = 0; i < fens.size(); ++i) {
        auto pos = chess::Position(fens.at(i));

        // Perft
        const auto t0 = std::chrono::steady_clock::now();
        const auto nodes = perft(pos, depth);
        const auto t1 = std::chrono::steady_clock::now();
        const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

        total_time += dt;
        total_nodes += nodes;

        // Print chart row
        std::cout << std::left;
        std::cout << std::setw(5) << i + 1;
        std::cout << std::right;
        std::cout << std::setw(10) << nodes;
        std::cout << std::setw(12) << total_nodes;
        std::cout << std::setw(9) << format_ms(dt);
        std::cout << std::setw(10) << format_ms(total_time);
        std::cout << std::left;
        std::cout << "  " << fens.at(i);
        std::cout << "\n";
    }

    return 0;
}

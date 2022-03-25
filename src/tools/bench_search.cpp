#include <array>
#include <atomic>
#include <chess/position.hpp>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <swizzles/search/root.hpp>
#include <swizzles/search/settings.hpp>

[[nodiscard]] auto format_ms(const std::chrono::milliseconds ms) noexcept -> std::string {
    const auto seconds = ms.count() / 1000;
    const auto milliseconds = ms.count() % 1000;
    return std::to_string(seconds) + "." + std::to_string((milliseconds / 100) % 10) +
           std::to_string((milliseconds / 10) % 10) + std::to_string(milliseconds % 10);
}

[[nodiscard]] auto format_eval(const int eval) noexcept -> std::string {
    return (eval >= 0 ? "+" : "-") + std::to_string(std::abs(eval / 100)) + "." +
           std::to_string(std::abs((eval / 10) % 10)) + std::to_string(std::abs(eval % 10));
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

    auto total_time = std::chrono::milliseconds(0);

    auto state = swizzles::uci::UCIState();
    state.tt = std::make_shared<TT<swizzles::TTEntry>>(1);
    // Search settings
    auto settings = swizzles::search::SearchSettings();
    settings.type = swizzles::search::SearchType::Depth;
    settings.depth = 1;
    std::atomic<bool> stop = false;

    // Get depth
    if (argc > 1) {
        settings.depth = std::stoi(argv[1]);
    }

    // Print chart title
    std::cout << std::left;
    std::cout << std::setw(5) << "Pos";
    std::cout << std::setw(6) << "Move";
    std::cout << std::right;
    std::cout << std::setw(8) << "Eval";
    std::cout << std::setw(10) << "Nodes";
    std::cout << std::setw(9) << "Time";
    std::cout << std::setw(10) << "Time Sum";
    std::cout << std::left;
    std::cout << "  FEN";
    std::cout << "\n";

    for (std::size_t i = 0; i < fens.size(); ++i) {
        state.pos.set_fen(fens.at(i));

        // Search
        const auto t0 = std::chrono::steady_clock::now();
        const auto results = swizzles::search::root(state, settings, stop);
        const auto t1 = std::chrono::steady_clock::now();
        const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

        total_time += dt;

        // Print chart row
        std::cout << std::left;
        std::cout << std::setw(5) << i + 1;
        std::cout << std::setw(6) << results.bestmove;
        std::cout << std::right;
        std::cout << std::setw(8) << format_eval(results.eval);
        std::cout << std::setw(10) << results.nodes;
        std::cout << std::setw(9) << format_ms(dt);
        std::cout << std::setw(10) << format_ms(total_time);
        std::cout << std::left;
        std::cout << "  " << fens.at(i);
        std::cout << "\n";
    }

    return 0;
}

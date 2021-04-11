#include <string>
#include "../../src/chess/position.hpp"
#include "../../src/search/controller.hpp"
#include "../../src/search/search.hpp"
#include "../catch.hpp"

TEST_CASE("Search - Checkmate") {
    const std::pair<std::string, std::string> tests[] = {
        {"3k4/8/3K4/8/5R2/8/8/8 w - - 0 1", "f4f8"},
        {"8/8/8/5r2/8/3k4/8/3K4 b - - 0 1", "f5f1"},
        {"1k1r2R1/8/1K6/8/8/8/8/8 w - - 0 1", "g8d8"},
        {"8/8/8/8/8/1k6/8/1K1R2r1 b - - 0 1", "g1d1"},
    };

    for (const auto &[fen, movestr] : tests) {
        INFO(fen);
        controller.reset();
        controller.set_depth(2);
        auto pos = chess::Position{fen};
        const auto [bestmove, _] = search::search(pos);
        REQUIRE(static_cast<std::string>(bestmove) == movestr);
    }
}

TEST_CASE("Search - Checkmate with castling") {
    const std::pair<std::string, std::string> tests[] = {
        {"8/8/8/8/8/8/7R/1k2K2R w K - 0 1", "e1g1"},
        {"1K2k2r/7r/8/8/8/8/8/8 b k - 0 1", "e8g8"},
        {"8/8/8/8/8/8/R7/R3K2k w Q - 0 1", "e1c1"},
        {"r3k2K/r7/8/8/8/8/8/8 b q - 0 1", "e8c8"},
    };

    for (const auto &[fen, movestr] : tests) {
        INFO(fen);
        controller.reset();
        controller.set_depth(2);
        auto pos = chess::Position{fen};
        const auto [bestmove, _] = search::search(pos);
        REQUIRE(static_cast<std::string>(bestmove) == movestr);
    }
}

TEST_CASE("Search - Checkmate 50moves priority") {
    const std::pair<std::string, std::string> tests[] = {
        {"7k/1R6/R7/8/8/8/8/4K3 w - - 99 1", "a6a8"},
        {"4k3/8/8/8/8/r7/1r6/7K b - - 99 1", "a3a1"},
    };

    for (const auto &[fen, movestr] : tests) {
        INFO(fen);
        controller.reset();
        controller.set_depth(2);
        auto pos = chess::Position{fen};
        const auto [bestmove, _] = search::search(pos);
        REQUIRE(static_cast<std::string>(bestmove) == movestr);
    }
}

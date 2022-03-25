#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <cstdint>
#include <string>
#include <swizzles/search/root.hpp>

TEST_SUITE_BEGIN("Search");

using pair_type = std::pair<std::string, std::string>;

TEST_CASE("Search - Checkmate") {
    const std::array<pair_type, 4> tests = {{
        {"3k4/8/3K4/8/5R2/8/8/8 w - - 0 1", "f4f8"},
        {"8/8/8/5r2/8/3k4/8/3K4 b - - 0 1", "f5f1"},
        {"1k1r2R1/8/1K6/8/8/8/8/8 w - - 0 1", "g8d8"},
        {"8/8/8/8/8/1k6/8/1K1R2r1 b - - 0 1", "g1d1"},
    }};

    auto state = swizzles::uci::UCIState();
    state.tt = std::make_shared<TT<swizzles::TTEntry>>(1);
    // Search settings
    auto settings = swizzles::search::SearchSettings();
    settings.type = swizzles::search::SearchType::Depth;
    settings.depth = 3;
    std::atomic<bool> stop = false;

    for (const auto &[fen, movestr] : tests) {
        INFO("FEN: ", fen);
        state.pos.set_fen(fen);
        const auto results = swizzles::search::root(state, settings, stop);
        REQUIRE(static_cast<std::string>(results.bestmove) == movestr);
    }
}

TEST_CASE("Search - Checkmate with castling") {
    const std::array<pair_type, 4> tests = {{
        {"8/8/8/8/8/8/7R/1k2K2R w K - 0 1", "e1h1"},
        {"1K2k2r/7r/8/8/8/8/8/8 b k - 0 1", "e8h8"},
        {"8/8/8/8/8/8/R7/R3K2k w Q - 0 1", "e1a1"},
        {"r3k2K/r7/8/8/8/8/8/8 b q - 0 1", "e8a8"},
    }};

    auto state = swizzles::uci::UCIState();
    state.tt = std::make_shared<TT<swizzles::TTEntry>>(1);
    // Search settings
    auto settings = swizzles::search::SearchSettings();
    settings.type = swizzles::search::SearchType::Depth;
    settings.depth = 3;
    std::atomic<bool> stop = false;

    for (const auto &[fen, movestr] : tests) {
        INFO("FEN: ", fen);
        state.pos.set_fen(fen);
        const auto results = swizzles::search::root(state, settings, stop);
        REQUIRE(static_cast<std::string>(results.bestmove) == movestr);
    }
}

/*
TEST_CASE("Search - Checkmate 50moves priority") {
    const std::array<pair_type, 2> tests = {{
        {"7k/1R6/R7/8/8/8/8/4K3 w - - 99 1", "a6a8"},
        {"4k3/8/8/8/8/r7/1r6/7K b - - 99 1", "a3a1"},
    }};

    // Search settings
    auto settings = swizzles::search::SearchSettings();
    settings.type = swizzles::search::SearchType::Depth;
    settings.depth = 3;
    std::atomic<bool> stop = false;

    for (const auto &[fen, movestr] : tests) {
        INFO("FEN: ", fen);
        settings.pos.set_fen(fen);
        const auto results = swizzles::search::root(settings, stop);
        REQUIRE(static_cast<std::string>(results.bestmove) == movestr);
    }
}
*/

TEST_SUITE_END();

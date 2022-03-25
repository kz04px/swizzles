#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <cstdint>
#include <string>
#include <swizzles/search/root.hpp>

TEST_SUITE_BEGIN("Search");

TEST_CASE("Search - Stalemate") {
    using pair_type = std::pair<std::string, std::string>;

    const std::array<pair_type, 6> tests = {{
        {"kb4r1/p7/8/8/8/6q1/8/R6K w - - 0 1", "a1a7"},
        {"r6k/8/6Q1/8/8/8/P7/KB4R1 b - - 0 1", "a8a2"},
        {"8/8/8/8/8/8/p7/k1K5 w - - 0 1", "c1c2"},
        {"K1k5/P7/8/8/8/8/8/8 b - - 0 1", "c8c7"},
        {"K1k5/P1q5/8/B7/8/8/8/8 w - - 0 1", "a5c7"},
        {"8/8/8/8/b7/8/p1Q5/k1K5 b - - 0 1", "a4c2"},
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

TEST_SUITE_END();

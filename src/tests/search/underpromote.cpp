#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <cstdint>
#include <string>
#include <swizzles/search/root.hpp>

TEST_SUITE_BEGIN("Search");

TEST_CASE("Search - Underpromotions") {
    using pair_type = std::pair<std::string, std::string>;

    const std::array<pair_type, 2> tests = {{
        {"6n1/5P1k/5Q2/8/8/8/8/7K w - - 0 1", "f7f8n"},
        {"7k/8/8/8/8/5q2/5p1K/6N1 b - - 0 1", "f2f1n"},
    }};

    auto state = swizzles::uci::UCIState();
    state.tt = std::make_shared<TT<swizzles::TTEntry>>(1);
    // Search settings
    auto settings = swizzles::search::SearchSettings();
    settings.type = swizzles::search::SearchType::Depth;
    settings.depth = 2;
    std::atomic<bool> stop = false;

    for (const auto &[fen, movestr] : tests) {
        INFO("FEN: ", fen);
        state.pos.set_fen(fen);
        const auto results = swizzles::search::root(state, settings, stop);
        REQUIRE(static_cast<std::string>(results.bestmove) == movestr);
    }
}

TEST_SUITE_END();

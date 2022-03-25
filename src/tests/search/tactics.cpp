#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <cstdint>
#include <string>
#include <swizzles/search/root.hpp>

TEST_SUITE_BEGIN("Search");

TEST_CASE("Search - Tactics") {
    using pair_type = std::pair<std::string, std::string>;

    const std::array<pair_type, 2> tests = {{
        {"4k3/q7/1P6/8/8/8/8/4K3 w - - 0 1", "b6a7"},
        {"4k3/8/8/3q4/8/8/3Q4/4K3 w - - 0 1", "d2d5"},
    }};

    auto state = swizzles::uci::UCIState();
    state.tt = std::make_shared<TT<swizzles::TTEntry>>(1);
    // Search settings
    auto settings = swizzles::search::SearchSettings();
    settings.type = swizzles::search::SearchType::Depth;
    settings.depth = 4;
    std::atomic<bool> stop = false;

    for (const auto &[fen, movestr] : tests) {
        INFO("FEN: ", fen);
        state.pos.set_fen(fen);
        const auto results = swizzles::search::root(state, settings, stop);
        REQUIRE(static_cast<std::string>(results.bestmove) == movestr);
    }
}

TEST_SUITE_END();

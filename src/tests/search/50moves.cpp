#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <cstdint>
#include <string>
#include <swizzles/search/root.hpp>

TEST_SUITE_BEGIN("Search");

TEST_CASE("Search - Fifty move draw") {
    using pair_type = std::pair<std::string, std::string>;

    const std::array<pair_type, 4> tests = {{
        {"7k/8/R7/1R6/7K/8/7P/8 w - - 99 1", "h2h3"},
        {"8/7p/8/7k/1r6/r7/8/7K b - - 99 1", "h7h6"},
        {"8/8/8/P7/8/6n1/3R4/R3K2k w Q - 99 1", "a5a6"},
        {"r3k2K/3r4/6N1/8/p7/8/8/8 b q - 99 1", "a4a3"},
    }};

    // Search settings
    auto settings = swizzles::search::SearchSettings();
    settings.type = swizzles::search::SearchType::Depth;
    settings.depth = 2;
    std::atomic<bool> stop = false;

    for (const auto &[fen, movestr] : tests) {
        INFO("FEN: ", fen);
        settings.pos.set_fen(fen);
        const auto results = swizzles::search::root(settings, stop);
        REQUIRE(static_cast<std::string>(results.bestmove) == movestr);
    }
}

TEST_SUITE_END();

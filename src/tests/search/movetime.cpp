#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <chrono>
#include <cstdint>
#include <swizzles/search/root.hpp>

TEST_SUITE_BEGIN("Search");

TEST_CASE("Search - Movetime") {
    constexpr std::array<int, 3> movetimes = {20, 100, 200};

    // Search settings
    auto settings = swizzles::search::SearchSettings();
    settings.type = swizzles::search::SearchType::Movetime;
    settings.pos.set_fen("startpos");
    std::atomic<bool> stop = false;

    for (const auto movetime : movetimes) {
        settings.movetime = movetime;
        const auto t0 = std::chrono::steady_clock::now();
        const auto results = swizzles::search::root(settings, stop);
        const auto t1 = std::chrono::steady_clock::now();
        const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
        REQUIRE(dt.count() <= movetime + 50);
        REQUIRE(results.bestmove != chess::Move());
    }
}

TEST_SUITE_END();

#include <string>
#include "../../src/chess/position.hpp"
#include "../../src/search/controller.hpp"
#include "../../src/search/search.hpp"
#include "../catch.hpp"

TEST_CASE("Search - Movetime") {
    constexpr std::array<int, 3> movetimes = {20, 100, 200};
    auto pos = chess::Position{"startpos"};

    for (const auto movetime : movetimes) {
        controller.reset();
        controller.set_movetime(movetime);
        const auto t0 = std::chrono::high_resolution_clock::now();
        const auto [bestmove, _] = search::search(pos);
        const auto t1 = std::chrono::high_resolution_clock::now();
        const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
        REQUIRE(dt.count() <= movetime + 50);
    }
}

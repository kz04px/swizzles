#include <string>
#include "../../src/chess/position.hpp"
#include "../../src/search/controller.hpp"
#include "../../src/search/search.hpp"
#include "../catch.hpp"

TEST_CASE("Search - Stalemate") {
    const std::pair<std::string, std::string> tests[] = {
        {"kb4r1/p7/8/8/8/6q1/8/R6K w - - 0 1", "a1a7"},
        {"r6k/8/6Q1/8/8/8/P7/KB4R1 b - - 0 1", "a8a2"},
        {"8/8/8/8/8/8/p7/k1K5 w - - 0 1", "c1c2"},
        {"K1k5/P7/8/8/8/8/8/8 b - - 0 1", "c8c7"},
        {"K1k5/P1q5/8/B7/8/8/8/8 w - - 0 1", "a5c7"},
        {"8/8/8/8/b7/8/p1Q5/k1K5 b - - 0 1", "a4c2"},
    };

    for (const auto &[fen, movestr] : tests) {
        INFO(fen);
        controller.reset();
        controller.set_depth(4);
        auto pos = chess::Position{fen};
        const auto [bestmove, _] = search::search(pos);
        REQUIRE(static_cast<std::string>(bestmove) == movestr);
    }
}

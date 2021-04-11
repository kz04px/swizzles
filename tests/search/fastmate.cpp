#include <string>
#include "../../src/chess/position.hpp"
#include "../../src/search/controller.hpp"
#include "../../src/search/search.hpp"
#include "../catch.hpp"

TEST_CASE("Search - Fast mate") {
    const std::pair<std::string, std::string> tests[] = {
        {"k7/pp6/8/8/8/8/3R4/K7 w - - 0 1", "d2d8"},
        {"k7/8/pp6/8/8/2R5/2R5/K7 w - - 0 1", "c3c8"},
        {"k7/6R1/5R1P/8/8/8/8/K7 w - - 0 1", "f6f8"},
    };

    for (const auto &[fen, movestr] : tests) {
        controller.reset();
        controller.set_depth(4);
        auto pos = chess::Position{fen};
        const auto [bestmove, ponder] = search::search(pos);
        REQUIRE(static_cast<std::string>(bestmove) == movestr);
    }
}

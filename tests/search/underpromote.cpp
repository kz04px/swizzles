#include <string>
#include "../../src/chess/position.hpp"
#include "../../src/search/controller.hpp"
#include "../../src/search/search.hpp"
#include "../catch.hpp"

TEST_CASE("Search - Underpromotions") {
    const std::pair<std::string, std::string> tests[] = {
        {"6n1/5P1k/5Q2/8/8/8/8/7K w - - 0 1", "f7f8n"},
        {"7k/8/8/8/8/5q2/5p1K/6N1 b - - 0 1", "f2f1n"},
    };

    for (const auto &[fen, movestr] : tests) {
        INFO(fen);
        controller.reset();
        controller.set_depth(2);
        auto pos = chess::Position{fen};
        const auto [bestmove, ponder] = search::search(pos);
        REQUIRE(static_cast<std::string>(bestmove) == movestr);
    }
}

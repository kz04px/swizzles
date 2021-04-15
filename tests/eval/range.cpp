#include <string>
#include "../../src/chess/position.hpp"
#include "../../src/eval/eval.hpp"
#include "../../src/search/constants.hpp"
#include "../catch.hpp"

TEST_CASE("Eval range") {
    const std::string fens[] = {
        "4k3/8/8/8/8/QQQQ4/QQQQ4/RNBQKBNR w KQ - 0 1",
    };

    for (const auto &fen : fens) {
        const auto pos = chess::Position{fen};
        const auto got = eval::eval(pos);

        INFO(fen);
        REQUIRE(std::abs(got) <= constants::max_eval);
    }
}

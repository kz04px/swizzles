#include <string>
#include "../../src/chess/position.hpp"
#include "../../src/eval/eval.hpp"
#include "../catch.hpp"

void flip(chess::Position &pos) {
}

TEST_CASE("Eval mirroring") {
    const std::string fens[] = {
        "startpos",
        "k7/3r4/3Pn3/2p1p3/1r1K3q/8/1Pn5/b5b1 w - - 0 1",
    };

    for (const auto &fen : fens) {
        auto pos = chess::Position{fen};
        const auto e1 = eval::eval(pos);
        flip(pos);
        const auto e2 = eval::eval(pos);

        INFO(fen);
        REQUIRE(e1 == e2);
    }
}

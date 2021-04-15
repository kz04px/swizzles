#include <string>
#include "../../src/chess/position.hpp"
#include "../../src/eval/eval.hpp"
#include "../catch.hpp"

TEST_CASE("Known evals") {
    std::pair<std::string, int> tests[] = {
        {"startpos", 10},
        {"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1", -31},
        {"rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2", 20},
        {"rnbqkb1r/pp2pppp/3p1n2/8/3NP3/2N5/PPP2PPP/R1BQKB1R b KQkq - 2 5", -59},
        {"4kn2/4n2p/2P5/8/2P2p1P/1RR2Q1P/1BB5/4K3 w - - 0 1", 2365},
        {"4k3/4p3/1P6/bp6/7q/4R3/3PPP2/4K3 w - - 0 1", -543},
    };

    for (const auto &[fen, score] : tests) {
        const auto pos = chess::Position{fen};
        const auto got = eval::eval(pos);

        INFO(fen);
        REQUIRE(got == score);
    }
}

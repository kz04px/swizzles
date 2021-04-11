#include "../src/uci/uci.hpp"
#include <sstream>
#include <string>
#include "../src/chess/position.hpp"
#include "catch.hpp"

TEST_CASE("UCI - ucinewgame") {
    auto pos = chess::Position{"rnbqkb1r/pp2pp1p/3p1np1/8/3NP3/2N5/PPP2PPP/R1BQKB1R w KQkq - 0 6"};
    uci::ucinewgame(pos);
    REQUIRE(pos.get_fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

TEST_CASE("UCI - position fen") {
    std::stringstream ss{"fen rnbqkb1r/pp2pp1p/3p1np1/8/3NP3/2N5/PPP2PPP/R1BQKB1R w KQkq - 0 6"};
    auto pos = chess::Position{};
    uci::position(pos, ss);
    REQUIRE(pos.get_fen() == "rnbqkb1r/pp2pp1p/3p1np1/8/3NP3/2N5/PPP2PPP/R1BQKB1R w KQkq - 0 6");
}

TEST_CASE("UCI - position startpos") {
    std::stringstream ss{"startpos"};
    auto pos = chess::Position{};
    uci::position(pos, ss);
    REQUIRE(pos.get_fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

TEST_CASE("UCI - moves") {
    std::stringstream ss{"e2e4 c7c5 g1f3 d7d6 d2d4 c5d4 f3d4 g8f6 b1c3 g7g6"};
    auto pos = chess::Position{"startpos"};
    uci::moves(pos, ss);
    REQUIRE(pos.get_fen() == "rnbqkb1r/pp2pp1p/3p1np1/8/3NP3/2N5/PPP2PPP/R1BQKB1R w KQkq - 0 6");
}

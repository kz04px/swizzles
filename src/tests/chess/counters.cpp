#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <sstream>
#include <string>

void makemove(chess::Position &pos, const std::string &movestr) {
    for (const auto move : pos.movegen()) {
        if (static_cast<std::string>(move) == movestr) {
            pos.makemove(move);
            return;
        }
    }
    throw std::invalid_argument("Couldn't find the move string: \"" + movestr + "\"");
}

struct Test {
    int halfmoves = 0;
    int fullmoves = 1;
    std::string moves;
};

TEST_CASE("Position counters") {
    const std::array<Test, 13> tests = {{
        {0, 1, "e2e4"},
        {0, 2, "e2e4 c7c5"},
        {1, 2, "e2e4 c7c5 g1f3"},
        {0, 3, "e2e4 c7c5 g1f3 d7d6"},
        {0, 3, "e2e4 c7c5 g1f3 d7d6 d2d4"},
        {0, 4, "e2e4 c7c5 g1f3 d7d6 d2d4 c5d4"},
        {0, 4, "e2e4 c7c5 g1f3 d7d6 d2d4 c5d4 f3d4"},
        {0, 5, "e2e4 c7c5 g1f3 d7d6 d2d4 c5d4 f3d4 a7a5"},
        {1, 5, "e2e4 c7c5 g1f3 d7d6 d2d4 c5d4 f3d4 a7a5 f1e2"},
        {0, 6, "e2e4 c7c5 g1f3 d7d6 d2d4 c5d4 f3d4 a7a5 f1e2 a5a4"},
        {0, 6, "e2e4 c7c5 g1f3 d7d6 d2d4 c5d4 f3d4 a7a5 f1e2 a5a4 b2b4"},
        {0, 7, "e2e4 c7c5 g1f3 d7d6 d2d4 c5d4 f3d4 a7a5 f1e2 a5a4 b2b4 a4b3"},
        {1, 7, "e2e4 c7c5 g1f3 d7d6 d2d4 c5d4 f3d4 a7a5 f1e2 a5a4 b2b4 a4b3 e1h1"},
    }};

    for (const auto &[half, full, movestrings] : tests) {
        INFO("Moves: " << movestrings);
        auto pos = chess::Position("startpos");

        std::stringstream ss(movestrings);
        std::string word;
        while (ss >> word) {
            REQUIRE_NOTHROW(makemove(pos, word));
        }

        REQUIRE(pos.halfmoves() == half);
        REQUIRE(pos.fullmoves() == full);
    }
}

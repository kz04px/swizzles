#include <string>
#include <vector>
#include "../../src/chess/position.hpp"
#include "../catch.hpp"

TEST_CASE("Position::checkers()") {
    const std::pair<chess::Bitboard, std::string> positions[] = {
        {0x0, "startpos"},
        {0x101482000440, "k7/3r4/3Pn3/2p1p3/1r1K3q/8/1Pn5/b5b1 w - - 0 1"},
    };

    for (const auto &[ans, fen] : positions) {
        chess::Position pos{fen};
        INFO(fen);
        REQUIRE(pos.checkers() == ans);
    }
}

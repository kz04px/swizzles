#include <string>
#include <vector>
#include "../../src/chess/position.hpp"
#include "../catch.hpp"

const std::string fens[] = {
    {"startpos"},
};

TEST_CASE("Position::is_capture()") {
    for (const auto &fen : fens) {
        chess::Position pos{fen};
        const auto moves = pos.legal_captures();
        INFO(fen);
        for (const auto &move : moves) {
            REQUIRE(pos.is_legal(move));
            REQUIRE(move.is_capturing());
        }
    }
}

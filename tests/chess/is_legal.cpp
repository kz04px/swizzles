#include <string>
#include <vector>
#include "../../src/chess/position.hpp"
#include "../catch.hpp"

const std::string fens[] = {
    {"startpos"},
};

TEST_CASE("Position::is_legal()") {
    for (const auto &fen : fens) {
        chess::Position pos{fen};
        const auto moves = pos.legal_moves();
        INFO(fen);
        for (const auto &move : moves) {
            REQUIRE(pos.is_legal(move));
        }
    }
}

#include "../src/search/see.hpp"
#include <sstream>
#include <string>
#include <tuple>
#include "../src/chess/position.hpp"
#include "../src/chess/square.hpp"
#include "catch.hpp"

[[nodiscard]] chess::Move find_move(const chess::Position &pos, const std::string &movestr) {
    const auto legal_moves = pos.legal_moves();
    for (const auto &move : legal_moves) {
        if (movestr == static_cast<std::string>(move)) {
            return move;
        }
    }
    return {};
}

TEST_CASE("SEEquiet") {
    std::tuple<std::string, std::string, int> tests[] = {
        {"startpos", "e2e4", 0},
        {"4k3/8/8/8/3Q4/8/8/4K3 w - - 0 1", "d4d7", -900},
        {"4k3/8/8/2q5/8/2PP4/8/4K3 w - - 0 1", "d3d4", 0},
        {"4k3/8/4n3/2q5/8/2QP4/8/4K3 w - - 0 1", "d3d4", -100},
        {"4k3/8/4n3/2b5/8/2QP4/8/4K3 w - - 0 1", "d3d4", -100},
    };

    for (const auto &[fen, movestr, value] : tests) {
        auto pos = chess::Position{fen};
        const auto move = find_move(pos, movestr);
        const auto got = search::SEEquiet(pos, move);

        INFO(fen);
        REQUIRE(move);
        REQUIRE(got == value);
    }
}

TEST_CASE("SEEcapture") {
    std::tuple<std::string, std::string, int> tests[] = {
        {"rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2", "e4d5", 0},
        {"rnbqkbnr/ppp1pppp/8/3p4/8/2N5/PPPPPPPP/R1BQKBNR w KQkq - 0 2", "c3d5", -220},
        {"2q1k3/8/8/2P5/1P6/8/8/4K3 b - - 0 1", "c8c5", -800},
        {"2q1k3/2r5/8/2N5/1B6/8/8/4K3 b - - 0 1", "c7c5", 150},
        {"2q1k3/8/3p4/2N5/1B6/8/8/4K3 b - - 0 1", "d6c5", 320},
        {"4k3/8/8/8/4Pp2/8/8/4K3 b - e3 0 1", "f4e3", 100},
    };

    for (const auto &[fen, movestr, value] : tests) {
        auto pos = chess::Position{fen};
        const auto move = find_move(pos, movestr);
        const auto got = search::SEEcapture(pos, move);

        INFO(fen);
        REQUIRE(move);
        REQUIRE(got == value);
    }
}

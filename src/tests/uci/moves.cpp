#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <sstream>
#include <string>
#include <swizzles/uci/uci.hpp>

TEST_SUITE_BEGIN("UCI");

using pair_type = std::pair<std::string, std::string>;

TEST_CASE("UCI - moves") {
    const std::array<pair_type, 7> tests = {{
        {"", "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"},
        {"e2e4", "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"},
        {"e2e4 c7c5", "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"},
        {"e2e3 e7e6 f1e2 f8e7 g1f3 g8f6 e1g1 e8g8", "rnbq1rk1/ppppbppp/4pn2/8/8/4PN2/PPPPBPPP/RNBQ1RK1 w - - 6 5"},
        {"e2e3 e7e6 f1e2 f8e7 g1f3 g8f6 e1h1 e8h8", "rnbq1rk1/ppppbppp/4pn2/8/8/4PN2/PPPPBPPP/RNBQ1RK1 w - - 6 5"},
        {"d2d3 d7d6 c1e3 c8e6 d1d2 d8d7 b1c3 b8c6 e1c1 e8c8",
         "2kr1bnr/pppqpppp/2npb3/8/8/2NPB3/PPPQPPPP/2KR1BNR w - - 8 6"},
        {"d2d3 d7d6 c1e3 c8e6 d1d2 d8d7 b1c3 b8c6 e1a1 e8a8",
         "2kr1bnr/pppqpppp/2npb3/8/8/2NPB3/PPPQPPPP/2KR1BNR w - - 8 6"},
    }};

    std::stringstream ss;

    for (const auto &[moves, fen] : tests) {
        INFO("Moves: ", moves);
        INFO("FEN: ", fen);
        auto pos = chess::Position("startpos");
        ss << moves;
        swizzles::uci::moves(ss, pos);
        REQUIRE(pos.get_fen() == fen);
        ss.clear();
    }
}

TEST_CASE("UCI - moves illegal") {
    const std::array<pair_type, 4> tests = {{
        {"0000", "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"},
        {"e2e4 0000", "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"},
        {"e2e4 c7c5 0000", "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"},
        {"e2e4 c7c5 0000 g1f3", "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"},
    }};

    std::stringstream ss;

    for (const auto &[moves, fen] : tests) {
        INFO("Moves: ", moves);
        INFO("FEN: ", fen);
        auto pos = chess::Position("startpos");
        ss << moves;
        swizzles::uci::moves(ss, pos);
        REQUIRE(pos.get_fen() == fen);
        ss.clear();
    }
}

TEST_SUITE_END();

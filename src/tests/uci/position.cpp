#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <sstream>
#include <string>
#include <swizzles/uci/uci.hpp>
#include <tuple>

TEST_SUITE_BEGIN("UCI");

TEST_CASE("UCI - \"position startpos\"") {
    std::stringstream ss("position startpos");
    swizzles::uci::UCIState state;
    swizzles::uci::parse(ss, state);
    REQUIRE(state.pos.get_fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

TEST_CASE("UCI - \"position [fen]\"") {
    const std::array<std::string, 8> fens = {{
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w K - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Q - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w k - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b q - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b - - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Kk - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Qq - 0 1",
    }};

    std::stringstream ss;
    swizzles::uci::UCIState state;

    SUBCASE("no \"moves\" list") {
        for (const auto &fen : fens) {
            ss << "position fen " << fen;
            swizzles::uci::parse(ss, state);
            REQUIRE(state.pos.get_fen() == fen);
            ss.clear();
        }
    }

    SUBCASE("empty \"moves\" list") {
        for (const auto &fen : fens) {
            ss << "position fen " << fen << " moves";
            swizzles::uci::parse(ss, state);
            REQUIRE(state.pos.get_fen() == fen);
            ss.clear();
        }
    }
}

TEST_CASE("UCI - \"position [fen] moves [moves]\"") {
    using tuple_type = std::tuple<std::string, std::string, std::string>;

    const std::array<tuple_type, 6> tests = {{
        {"startpos", "", "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"},
        {"startpos", "e2e4", "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"},
        {"startpos", "e2e4 c7c5", "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
         "",
         "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
         "e2e4",
         "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
         "e2e4 c7c5",
         "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"},
    }};

    swizzles::uci::UCIState state;

    for (const auto &[fen_start, moves, fen_end] : tests) {
        INFO("FEN: ", fen_start);
        INFO("Moves: ", moves);

        std::stringstream ss;
        if (fen_start == "startpos") {
            ss << "position startpos moves " << moves;
        } else {
            ss << "position fen " << fen_start << " moves " << moves;
        }
        swizzles::uci::parse(ss, state);

        REQUIRE(state.pos.get_fen() == fen_end);
    }
}

TEST_SUITE_END();

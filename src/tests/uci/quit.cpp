#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <sstream>
#include <string>
#include <swizzles/uci/uci.hpp>

TEST_SUITE_BEGIN("UCI");

TEST_CASE("UCI - no quit") {
    const std::array<std::string, 7> inputs = {{
        "position startpos",
        "position startpos moves",
        "position startpos moves e2e4 c7c5",
        "position rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "position rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves",
        "position rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves e2e4 c7c5",
        "setoption name test value true",
    }};

    std::stringstream ss;
    auto pos = chess::Position();

    for (const auto &input : inputs) {
        ss << input;
        const auto quit = swizzles::uci::parse(ss, pos);
        REQUIRE(!quit);
        ss.clear();
    }
}

TEST_CASE("UCI - quit") {
    std::stringstream ss("quit");
    auto pos = chess::Position();
    const auto quit = swizzles::uci::parse(ss, pos);
    REQUIRE(quit);
}

TEST_SUITE_END();

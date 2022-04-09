#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <chess/see.hpp>
#include <optional>
#include <string>
#include <tuple>

[[nodiscard]] static auto find_move(const chess::Position &pos, const std::string &movestring) noexcept
    -> std::optional<chess::Move> {
    const auto moves = pos.movegen();
    for (const auto &move : moves) {
        if (movestring == static_cast<std::string>(move)) {
            return move;
        }
    }
    return {};
}

TEST_CASE("SEE") {
    using tuple_type = std::tuple<std::string, std::string, int>;

    const std::array<tuple_type, 11> tests = {{
        {"startpos", "e2e4", 0},
        {"startpos", "g1f3", 0},
        {"4k3/8/8/4r3/5P2/8/8/4K3 w - - 0 1", "f4e5", 500},
        {"4k3/8/3p4/4n3/8/4R3/8/4K3 w - - 0 1", "e3e5", -200},
        {"4k3/8/1p1p4/2p5/3P4/8/2R5/4K3 w - - 0 1", "d4c5", 0},
        {"4k3/8/1q1p4/2p5/3P4/8/2R5/4K3 w - - 0 1", "d4c5", 0},
        {"4k3/8/1q1p4/2p5/3P4/8/2R5/4K3 w - - 0 1", "c2c5", -400},
        {"4k3/8/1q1p4/2p5/3P4/8/2R5/2Q1K3 w - - 0 1", "d4c5", 100},
        {"4k3/8/1b1p4/2p5/3P4/4B3/5B2/4K3 w - - 0 1", "d4c5", 100},
        {"4k3/8/1b1p4/2p5/3P4/4Q3/5B2/4K3 w - - 0 1", "d4c5", 0},
        {"8/8/8/2pk4/3P4/4P3/8/4K3 b - - 0 1", "c5d4", 100},
    }};

    const std::array<int, 7> values = {100, 300, 300, 500, 900, 1'000'000, 0};

    for (const auto &[fen, movestring, score] : tests) {
        INFO("FEN: ", fen);
        INFO("Move: ", movestring);
        const auto pos = chess::Position(fen);
        const auto move = find_move(pos, movestring);
        REQUIRE(move);
        REQUIRE(chess::see_move(pos, *move, values) == score);
    }
}

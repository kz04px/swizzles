#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <cstdint>
#include <string>

TEST_SUITE_BEGIN("ShallowPerft");

[[nodiscard]] static auto perft(chess::Position &pos, const std::size_t depth) noexcept -> std::uint64_t {
    if (depth == 0) {
        return 1ULL;
    }

    std::uint64_t nodes = 0ULL;
    const auto moves = pos.movegen();

    REQUIRE(moves.size() <= moves.capacity());

    for (const auto &move : moves) {
        pos.makemove<false>(move);

        if (pos.is_attacked(pos.get_king(!pos.turn()), pos.turn())) {
            pos.undomove();
            continue;
        }

        nodes += perft(pos, depth - 1);
        pos.undomove();
    }

    return nodes;
}

using pair_type = std::pair<std::string, std::vector<std::uint64_t>>;

TEST_CASE("Perft - Many moves") {
    INFO("Perft position with the most known moves");
    const auto pos = chess::Position("R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNN1KB1 w - - 0 1");
    const auto moves = pos.movegen();
    REQUIRE(moves.size() == 218);
    REQUIRE(moves.size() <= moves.capacity());
}

TEST_CASE("Perft - Shallow") {
    const std::array<pair_type, 34> tests = {{
        {"startpos", {20, 400, 8902, 197281}},
        {"bqnb1rkr/pp3ppp/3ppn2/2p5/5P2/P2P4/NPP1P1PP/BQ1BNRKR w HFhf - 2 9", {21, 528, 12189, 326672}},
        {"2nnrbkr/p1qppppp/8/1ppb4/6PP/3PP3/PPP2P2/BQNNRBKR w HEhe - 1 9", {21, 807, 18002, 667366}},
        {"b1q1rrkb/pppppppp/3nn3/8/P7/1PPP4/4PPPP/BQNNRKRB w GE - 1 9", {20, 479, 10471, 273318}},
        {"qbbnnrkr/2pp2pp/p7/1p2pp2/8/P3PP2/1PPP1KPP/QBBNNR1R w hf - 0 9", {22, 593, 13440, 382958}},
        {"1nbbnrkr/p1p1ppp1/3p4/1p3P1p/3Pq2P/8/PPP1P1P1/QNBBNRKR w HFhf - 0 9", {28, 1120, 31058, 1171749}},
        {"qnbnr1kr/ppp1b1pp/4p3/3p1p2/8/2NPP3/PPP1BPPP/QNB1R1KR w HEhe - 1 9", {29, 899, 26578, 824055}},
        {"r1kbbqrn/ppp3pp/2np1p2/1P2p3/3P1P2/8/P1P1P1PP/RNKBBQRN w GAga - 0 9", {32, 920, 28916, 844881}},
        {"rk1qbbrn/p2npppp/1p6/2p4Q/8/4P3/PPPP1PPP/RNK1B1RN w GA - 2 9", {35, 657, 22359, 495406}},
        {"4k3/b7/8/2Pp4/8/8/8/6K1 w - d6 0 2", {5}},
        {"4k3/7b/8/4pP2/8/8/8/1K6 w - e6 0 2", {5}},
        {"6k1/8/8/8/2pP4/8/B7/3K4 b - d3 0 2", {5}},
        {"1k6/8/8/8/4Pp2/8/7B/4K3 b - e3 0 2", {5}},
        {"4k3/b7/8/1pP5/8/8/8/6K1 w - b6 0 2", {6}},
        {"4k3/7b/8/5Pp1/8/8/8/1K6 w - g6 0 2", {6}},
        {"6k1/8/8/8/1Pp5/8/B7/4K3 b - b3 0 2", {6}},
        {"1k6/8/8/8/5pP1/8/7B/4K3 b - g3 0 2", {6}},
        {"4k3/K7/8/1pP5/8/8/8/6b1 w - b6 0 2", {6}},
        {"4k3/7K/8/5Pp1/8/8/8/1b6 w - g6 0 2", {6}},
        {"6B1/8/8/8/1Pp5/8/k7/4K3 b - b3 0 2", {6}},
        {"1B6/8/8/8/5pP1/8/7k/4K3 b - g3 0 2", {6}},
        {"4k3/8/8/K2pP2r/8/8/8/8 w - d6 0 1", {6}},
        {"4k3/8/8/r2pP2K/8/8/8/8 w - d6 0 1", {6}},
        {"8/8/8/8/1k1Pp2R/8/8/4K3 b - d3 0 1", {8}},
        {"8/8/8/8/1R1Pp2k/8/8/4K3 b - d3 0 1", {6}},
        {"k7/8/4r3/3pP3/8/8/8/4K3 w - d6 0 1", {5}},
        {"k3K3/8/8/3pP3/8/8/8/4r3 w - d6 0 1", {6}},
        {"8/8/8/8/1k1PpN1R/8/8/4K3 b - d3 0 1", {9, 193, 1322}},
        {"8/8/8/8/1k1Ppn1R/8/8/4K3 b - d3 0 1", {17, 220, 3001}},
        {"4k3/8/8/2PpP3/8/8/8/4K3 w - d6 0 1", {9, 47, 376}},
        {"4k3/8/8/8/2pPp3/8/8/4K3 b - d3 0 1", {9, 47, 376}},
        {"4k3/8/8/4pP2/3K4/8/8/8 w - e6 0 2", {9}},
        {"8/8/8/4k3/5Pp1/8/8/3K4 b - f3 0 1", {9}},
        {"4k3/8/K6r/3pP3/8/8/8/8 w - d6 0 1", {6}},
    }};

    for (const auto &[fen, nodes] : tests) {
        INFO("FEN: ", fen);
        auto pos = chess::Position(fen);
        for (std::size_t i = 0; i < nodes.size(); ++i) {
            INFO("Depth: ", i);
            REQUIRE(perft(pos, i + 1) == nodes.at(i));
        }
    }
}

TEST_SUITE_END();

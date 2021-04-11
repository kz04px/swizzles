#include "../../src/chess/movegen.hpp"
#include <tuple>
#include "../../src/chess/bitboard.hpp"
#include "../../src/chess/square.hpp"
#include "../catch.hpp"

using namespace chess;

TEST_CASE("Movegen knights") {
    const std::pair<Square, Bitboard> positions[] = {
        {squares::A1, 0x20400},
        {squares::D4, 0x142200221400},
        {squares::F8, 0x88500000000000},
    };

    for (const auto& [sq, moves] : positions) {
        INFO(sq);
        REQUIRE(movegen::knight_moves(sq) == moves);
    }
}

TEST_CASE("Movegen bishops") {
    const std::tuple<Square, Bitboard, Bitboard> positions[] = {
        {squares::A1, 0x0, 0x8040201008040200},
        {squares::D4, 0x0, 0x8041221400142241},
        {squares::D4, 0x40000400040000, 0x40201400142040},
    };

    for (const auto& [sq, blockers, moves] : positions) {
        INFO(sq);
        REQUIRE(movegen::bishop_moves(sq, blockers) == moves);
    }
}

TEST_CASE("Movegen rooks") {
    const std::tuple<Square, Bitboard, Bitboard> positions[] = {
        {squares::A1, 0x0, 0x1010101010101fe},
        {squares::D4, 0x0, 0x8080808f7080808},
        {squares::D4, 0x840000800, 0x877080800},
    };

    for (const auto& [sq, blockers, moves] : positions) {
        INFO(sq);
        REQUIRE(movegen::rook_moves(sq, blockers) == moves);
    }
}

TEST_CASE("Movegen queens") {
    const std::pair<Square, Bitboard> positions[] = {
        {squares::A1, 0x0},
        {squares::D8, 0x8a00024008140000},
    };

    for (const auto& [sq, blockers] : positions) {
        const auto bishop = movegen::bishop_moves(sq, blockers);
        const auto rook = movegen::rook_moves(sq, blockers);
        const auto queen = movegen::queen_moves(sq, blockers);
        INFO(sq);
        REQUIRE((bishop | rook) == queen);
    }
}

TEST_CASE("Movegen king") {
    const std::pair<Square, Bitboard> positions[] = {
        {squares::A1, 0x302},
        {squares::D4, 0x1c141c0000},
        {squares::D8, 0x141c000000000000},
    };

    for (const auto& [sq, moves] : positions) {
        INFO(sq);
        REQUIRE(movegen::king_moves(sq) == moves);
    }
}

#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <chess/validate.hpp>
#include <string>

TEST_SUITE_BEGIN("Validate");

void validate(chess::Position &pos, const int depth) {
    REQUIRE_NOTHROW(chess::validate::position(pos));

    if (depth == 0) {
        return;
    }

    const auto moves = pos.movegen();
    for (const auto &move : moves) {
        REQUIRE_NOTHROW(chess::validate::move(move));

        pos.makemove(move);

        if (pos.is_attacked(pos.get_king(!pos.turn()), pos.turn())) {
            pos.undomove();
            continue;
        }

        validate(pos, depth - 1);
        pos.undomove();
    }
}

TEST_CASE("Validate moves & positions") {
    const std::array<std::string, 11> fens = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w K - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Q - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w k - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b q - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b - - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Kk - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b Qq - 0 1",
        "2rq1rk1/pp1bppbp/2np1np1/8/3NP3/1BN1BP2/PPPQ2PP/2KR3R b - - 8 11",
        "2rqr1k1/pp1bppbp/3p1np1/4n3/3NP2P/1BN1BP2/PPPQ2P1/1K1R3R b - - 0 13",
        "rnbqkbnr/ppp1pppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 3",
    };

    for (const auto &fen : fens) {
        INFO("FEN: ", fen);
        auto pos = chess::Position(fen);
        validate(pos, 3);
    }
}

TEST_SUITE_END();

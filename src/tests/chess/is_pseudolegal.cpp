#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <string>
#include <unordered_set>

static auto create_moveset(std::unordered_set<chess::Move> &moves_set,
                           chess::Position &pos,
                           const std::size_t depth) noexcept -> void {
    REQUIRE(!pos.is_pseudolegal(chess::Move()));

    if (depth == 0) {
        return;
    }

    const auto moves = pos.movegen();
    for (const auto &move : moves) {
        REQUIRE(pos.is_pseudolegal(move));

        moves_set.insert(move);

        pos.makemove<false>(move);
        if (pos.is_attacked(pos.get_king(!pos.turn()), pos.turn())) {
            pos.undomove();
            continue;
        }

        create_moveset(moves_set, pos, depth - 1);
        pos.undomove();
    }
}

TEST_CASE("Weak is_pseudolegal") {
    const std::array<std::string, 30> fens = {
        "startpos",
        "2rq1rk1/pp1bppbp/2np1np1/8/3NP3/1BN1BP2/PPPQ2PP/2KR3R b - - 8 11",
        "2rqr1k1/pp1bppbp/3p1np1/4n3/3NP2P/1BN1BP2/PPPQ2P1/1K1R3R b - - 0 13",
        "rnbqkbnr/ppp1pppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 3",
        "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R w KQ - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R b kq - 0 1",
        "r3k2r/2P3P1/8/8/8/8/2p3p1/R3K2R w KQkq - 0 1",
        "r3k2r/2P3P1/8/8/8/8/2p3p1/R3K2R b KQkq - 0 1",
        "r3k2r/8/2B5/8/8/2b5/8/R3K2R w KQkq - 0 1",
        "r3k2r/8/2B5/8/8/2b5/8/R3K2R b KQkq - 0 1",
        "r2NkN1r/8/8/8/8/8/8/R2nKn1R w KQkq - 0 1",
        "r2NkN1r/8/8/8/8/8/8/R2nKn1R b KQkq - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R b - - 0 1",
        "nnn1k3/1P6/8/8/8/8/1p6/NNN1K3 w - - 0 1",
        "nnn1k3/1P6/8/8/8/8/1p6/NNN1K3 b - - 0 1",
        "4k3/1P6/8/8/8/8/1p6/4K3 w - - 0 1",
        "4k3/1P6/8/8/8/8/1p6/4K3 b - - 0 1",
        "4k3/8/8/8/3pPp2/8/8/4K3 b - e3 0 1",
        "4k3/8/8/8/3pPp2/8/8/4K3 b - - 0 1",
        "7k/8/8/3Q4/3q4/8/8/7K w - - 0 1",
        "7k/8/8/pbbQNRP1/PBBqnrp1/8/8/7K w - - 0 1",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w HAha - 0 1",
        "1r2k1r1/8/8/8/8/8/8/1R2K1R1 w KQkq - 0 1",
        "1r2k1r1/8/8/8/8/8/8/1R2K1R1 w GBgb - 0 1",
        "1r3kr1/8/8/8/8/8/8/1R3KR1 w GBgb - 0 1",
        "2r3kr/8/8/8/8/8/8/3RKR2 w FDhc - 0 1",
        "rr3k2/8/8/8/8/8/8/RR3K2 w Bb - 0 1",
    };

    std::unordered_set<chess::Move> moves_set;

    // Create set of pseudolegal moves
    for (const auto &fen : fens) {
        auto pos = chess::Position(fen);
        create_moveset(moves_set, pos, 4);
    }

    REQUIRE(std::find(moves_set.begin(), moves_set.end(), chess::Move()) == moves_set.end());

    for (const auto &fen : fens) {
        INFO("FEN: ", fen);
        const auto pos = chess::Position(fen);
        const auto moves = pos.movegen();

        for (const auto move : moves_set) {
            INFO("Move: ", move);
            const auto is_generated = std::find(moves.begin(), moves.end(), move) != moves.end();
            const auto is_pseudolegal = pos.is_pseudolegal(move);
            REQUIRE(is_generated == is_pseudolegal);
        }
    }
}

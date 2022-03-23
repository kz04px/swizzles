#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <string>

TEST_CASE("FEN") {
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

    SUBCASE("Parse") {
        for (const auto &fen : fens) {
            INFO("FEN: ", fen);
            const auto pos = chess::Position(fen);
            CHECK(pos.get_fen() == fen);
        }
    }

    SUBCASE("Overwrite") {
        auto pos = chess::Position();
        for (const auto &fen : fens) {
            INFO("FEN: ", fen);
            pos.set_fen(fen);
            CHECK(pos.get_fen() == fen);
        }
    }

    SUBCASE("Startpos") {
        INFO("Make sure \"startpos\" is parsed correctly");
        CHECK(chess::Position("startpos").get_fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
}

TEST_CASE("FEN - 960") {
    SUBCASE("Parse files") {
        const std::array<std::string, 14> fens = {
            "bqnb1rkr/pp3ppp/3ppn2/2p5/5P2/P2P4/NPP1P1PP/BQ1BNRKR w HFhf - 2 9",
            "2nnrbkr/p1qppppp/8/1ppb4/6PP/3PP3/PPP2P2/BQNNRBKR w HEhe - 1 9",
            "b1q1rrkb/pppppppp/3nn3/8/P7/1PPP4/4PPPP/BQNNRKRB w GE - 1 9",
            "qbbnnrkr/2pp2pp/p7/1p2pp2/8/P3PP2/1PPP1KPP/QBBNNR1R w hf - 0 9",
            "1nbbnrkr/p1p1ppp1/3p4/1p3P1p/3Pq2P/8/PPP1P1P1/QNBBNRKR w HFhf - 0 9",
            "qnbnr1kr/ppp1b1pp/4p3/3p1p2/8/2NPP3/PPP1BPPP/QNB1R1KR w HEhe - 1 9",
            "q1bnrkr1/ppppp2p/2n2p2/4b1p1/2NP4/8/PPP1PPPP/QNB1RRKB w ge - 1 9",
            "qbn1brkr/ppp1p1p1/2n4p/3p1p2/P7/6PP/QPPPPP2/1BNNBRKR w HFhf - 0 9",
            "qnnbbrkr/1p2ppp1/2pp3p/p7/1P5P/2NP4/P1P1PPP1/Q1NBBRKR w HFhf - 0 9",
            "qn1rbbkr/ppp2p1p/1n1pp1p1/8/3P4/P6P/1PP1PPPK/QNNRBB1R w hd - 2 9",
            "qnr1bkrb/pppp2pp/3np3/5p2/8/P2P2P1/NPP1PP1P/QN1RBKRB w GDg - 3 9",
            "qb1nrkbr/1pppp1p1/1n3p2/p1B4p/8/3P1P1P/PPP1P1P1/QBNNRK1R w HEhe - 0 9",
            "qnnbrk1r/1p1ppbpp/2p5/p4p2/2NP3P/8/PPP1PPP1/Q1NBRKBR w HEhe - 0 9",
            "1qnrkbbr/1pppppp1/p1n4p/8/P7/1P1N1P2/2PPP1PP/QN1RKBBR w HDhd - 0 9",
        };

        for (const auto &fen : fens) {
            INFO("FEN: ", fen);
            const auto pos = chess::Position(fen);
            CHECK(pos.get_fen(true) == fen);
        }
    }

    SUBCASE("Parse KQkq") {
        using pair_type = std::pair<std::string, std::string>;

        const std::array<pair_type, 5> tests = {{
            {"bbqnnrkr/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w KQkq - 0 1",
             "bbqnnrkr/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w HFhf - 0 1"},
            {"bnnbrqkr/pppppppp/8/8/8/8/PPPPPPPP/BNNBRQKR w KQkq - 0 1",
             "bnnbrqkr/pppppppp/8/8/8/8/PPPPPPPP/BNNBRQKR w HEhe - 0 1"},
            {"bqrknbnr/pppppppp/8/8/8/8/PPPPPPPP/BQRKNBNR w KQkq - 0 1",
             "bqrknbnr/pppppppp/8/8/8/8/PPPPPPPP/BQRKNBNR w HChc - 0 1"},
            {"rknbbrqn/pppppppp/8/8/8/8/PPPPPPPP/RKNBBRQN w KQkq - 0 1",
             "rknbbrqn/pppppppp/8/8/8/8/PPPPPPPP/RKNBBRQN w FAfa - 0 1"},
            {"rkbrnbnq/pppppppp/8/8/8/8/PPPPPPPP/RKBRNBNQ w KQkq - 0 1",
             "rkbrnbnq/pppppppp/8/8/8/8/PPPPPPPP/RKBRNBNQ w DAda - 0 1"},
        }};

        for (const auto &[fen_in, fen_out] : tests) {
            INFO("FEN: ", fen_in);
            const auto pos = chess::Position(fen_in);
            CHECK(pos.get_fen(true) == fen_out);
        }
    }

    SUBCASE("Startpos") {
        INFO("Get startpos as a 960 fen");
        CHECK(chess::Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1").get_fen(true) ==
              "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w HAha - 0 1");
    }
}

TEST_CASE("FEN - Double 960") {
    SUBCASE("Parse files") {
        const std::array<std::string, 8> fens = {
            "qnnrkrbb/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w HFfd - 0 1",
            "rbknbrnq/pppppppp/8/8/8/8/PPPPPPPP/BQNBRNKR w HEfa - 0 1",
            "qrkbnnbr/pppppppp/8/8/8/8/PPPPPPPP/BBNRKQRN w GDhb - 0 1",
            "rknqbbrn/pppppppp/8/8/8/8/PPPPPPPP/RNBKNBQR w HAga - 0 1",
            "brkqnnrb/pppppppp/8/8/8/8/PPPPPPPP/BRQKNBNR w HBgb - 0 1",
            "nnqrbkrb/pppppppp/8/8/8/8/PPPPPPPP/RBKNQRBN w FAgd - 0 1",
            "qrnkbbnr/pppppppp/8/8/8/8/PPPPPPPP/RKBBNRNQ w FAhb - 0 1",
            "bbqnnrkr/pppppppp/8/8/8/8/PPPPPPPP/RQKRBBNN w DAhf - 0 1",
        };

        for (const auto &fen : fens) {
            INFO("FEN: ", fen);
            const auto pos = chess::Position(fen);
            CHECK(pos.get_fen(true) == fen);
        }
    }

    SUBCASE("Parse KQkq") {
        using pair_type = std::pair<std::string, std::string>;

        const std::array<pair_type, 12> tests = {{
            {"qnnrkrbb/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w KQkq - 0 1",
             "qnnrkrbb/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w HFfd - 0 1"},
            {"rbknbrnq/pppppppp/8/8/8/8/PPPPPPPP/BQNBRNKR w KQkq - 0 1",
             "rbknbrnq/pppppppp/8/8/8/8/PPPPPPPP/BQNBRNKR w HEfa - 0 1"},
            {"qrkbnnbr/pppppppp/8/8/8/8/PPPPPPPP/BBNRKQRN w KQkq - 0 1",
             "qrkbnnbr/pppppppp/8/8/8/8/PPPPPPPP/BBNRKQRN w GDhb - 0 1"},
            {"rknqbbrn/pppppppp/8/8/8/8/PPPPPPPP/RNBKNBQR w KQkq - 0 1",
             "rknqbbrn/pppppppp/8/8/8/8/PPPPPPPP/RNBKNBQR w HAga - 0 1"},
            {"brkqnnrb/pppppppp/8/8/8/8/PPPPPPPP/BRQKNBNR w KQkq - 0 1",
             "brkqnnrb/pppppppp/8/8/8/8/PPPPPPPP/BRQKNBNR w HBgb - 0 1"},
            {"nnqrbkrb/pppppppp/8/8/8/8/PPPPPPPP/RBKNQRBN w KQkq - 0 1",
             "nnqrbkrb/pppppppp/8/8/8/8/PPPPPPPP/RBKNQRBN w FAgd - 0 1"},
            {"qrnkbbnr/pppppppp/8/8/8/8/PPPPPPPP/RKBBNRNQ w KQkq - 0 1",
             "qrnkbbnr/pppppppp/8/8/8/8/PPPPPPPP/RKBBNRNQ w FAhb - 0 1"},
            {"bbqnnrkr/pppppppp/8/8/8/8/PPPPPPPP/RQKRBBNN w KQkq - 0 1",
             "bbqnnrkr/pppppppp/8/8/8/8/PPPPPPPP/RQKRBBNN w DAhf - 0 1"},
            {"qnnrkrbb/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w Qkq - 0 1",
             "qnnrkrbb/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w Ffd - 0 1"},
            {"qnnrkrbb/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w Kkq - 0 1",
             "qnnrkrbb/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w Hfd - 0 1"},
            {"qnnrkrbb/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w KQq - 0 1",
             "qnnrkrbb/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w HFd - 0 1"},
            {"qnnrkrbb/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w KQk - 0 1",
             "qnnrkrbb/pppppppp/8/8/8/8/PPPPPPPP/BBQNNRKR w HFf - 0 1"},
        }};

        for (const auto &[fen_in, fen_out] : tests) {
            INFO("FEN: ", fen_in);
            const auto pos = chess::Position(fen_in);
            CHECK(pos.get_fen(true) == fen_out);
        }
    }
}

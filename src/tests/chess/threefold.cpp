#include <doctest/doctest.h>
#include <array>
#include <chess/position.hpp>
#include <sstream>
#include <string>

inline void makemove(chess::Position &pos, const std::string &movestr) {
    for (const auto move : pos.movegen()) {
        if (static_cast<std::string>(move) == movestr) {
            pos.makemove(move);
            return;
        }
    }
    throw std::invalid_argument("Couldn't find the move string: \"" + movestr + "\"");
}

TEST_CASE("Position::num_repeats()") {
    using pair_type = std::pair<std::string, int>;

    const std::array<pair_type, 24> tests = {{
        // Test sequence
        {"", 1},
        {"g1f3", 1},
        {"g1f3 g8f6", 1},
        {"g1f3 g8f6 f3g1", 1},
        {"g1f3 g8f6 f3g1 f6g8", 2},
        {"g1f3 g8f6 f3g1 f6g8 g1f3", 2},
        {"g1f3 g8f6 f3g1 f6g8 g1f3 g8f6", 2},
        {"g1f3 g8f6 f3g1 f6g8 g1f3 g8f6 f3g1", 2},
        {"g1f3 g8f6 f3g1 f6g8 g1f3 g8f6 f3g1 f6g8", 3},
        {"g1f3 g8f6 f3g1 f6g8 g1f3 g8f6 f3g1 f6g8 b1c3", 1},
        {"g1f3 g8f6 f3g1 f6g8 g1f3 g8f6 f3g1 f6g8 b1c3 b8c6", 1},
        {"g1f3 g8f6 f3g1 f6g8 g1f3 g8f6 f3g1 f6g8 b1c3 b8c6 c3b1", 1},
        {"g1f3 g8f6 f3g1 f6g8 g1f3 g8f6 f3g1 f6g8 b1c3 b8c6 c3b1 c6b8", 4},
        // Test castling permissions
        {"", 1},
        {"g1f3", 1},
        {"g1f3 g8f6", 1},
        {"g1f3 g8f6 h1g1", 1},
        {"g1f3 g8f6 h1g1 f6g8", 1},
        {"g1f3 g8f6 h1g1 f6g8 g1h1", 1},
        {"g1f3 g8f6 h1g1 f6g8 g1h1 g8f6", 1},
        {"g1f3 g8f6 h1g1 f6g8 g1h1 g8f6 f3g1", 1},
        {"g1f3 g8f6 h1g1 f6g8 g1h1 g8f6 f3g1 f6g8", 1},
        // Test en passant
        {"e2e4 b8c6 e4e5 f7f5", 1},
        {"e2e4 b8c6 e4e5 f7f5 g1f3 c6b8 f3g1 b8c6", 1},
    }};

    for (const auto &[movestrings, repeats] : tests) {
        auto pos = chess::Position("startpos");

        std::stringstream ss(movestrings);
        std::string word;
        while (ss >> word) {
            REQUIRE_NOTHROW(makemove(pos, word));
        }

        REQUIRE(pos.num_repeats() == repeats);
    }
}

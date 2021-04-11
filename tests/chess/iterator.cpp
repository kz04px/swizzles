#include <cstdint>
#include "../../src/chess/bitboard.hpp"
#include "../catch.hpp"

const std::uint64_t masks[] = {
    0x0,
    0x101010101010101,
};

TEST_CASE("BitboardIterator") {
    for (const auto& mask : masks) {
        chess::Bitboard bb{mask};
        chess::Bitboard nbb;

        for (const auto& sq : bb) {
            nbb ^= sq;
        }

        REQUIRE(bb == nbb);
    }
}

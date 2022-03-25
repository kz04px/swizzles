#ifndef CHESS_PASSED_HPP
#define CHESS_PASSED_HPP

#include "bitboard.hpp"
#include "colour.hpp"

namespace chess {

template <Colour us>
[[nodiscard]] constexpr auto get_passed(const Bitboard our_pawns, const Bitboard their_pawns) noexcept -> Bitboard {
    if constexpr (us == Colour::White) {
        const auto attacks = their_pawns | their_pawns.south().west() | their_pawns.south().east();
        const auto rays = attacks.extend_south();
        return our_pawns & ~rays;
    } else {
        const auto attacks = their_pawns | their_pawns.north().west() | their_pawns.north().east();
        const auto rays = attacks.extend_north();
        return our_pawns & ~rays;
    }
}

static_assert(get_passed<Colour::White>(Bitboard(0x12000000ULL), Bitboard(0x0ULL)) == Bitboard(0x12000000ULL));
static_assert(get_passed<Colour::White>(Bitboard(0x12000000ULL), Bitboard(0xC0010008002000ULL)) ==
              Bitboard(0x10000000ULL));

static_assert(get_passed<Colour::Black>(Bitboard(0x12000000ULL), Bitboard(0x0ULL)) == Bitboard(0x12000000ULL));
static_assert(get_passed<Colour::Black>(Bitboard(0x12000000ULL), Bitboard(0xC0010008002000ULL)) ==
              Bitboard(0x2000000ULL));

}  // namespace chess

#endif

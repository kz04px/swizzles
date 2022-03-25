#ifndef CHESS_BITBOARD_HPP
#define CHESS_BITBOARD_HPP

#include <bit>
#include <cstdint>
#include <ostream>
#include "colour.hpp"
#include "square.hpp"

namespace chess {

enum Bitmask : std::uint64_t
{
    // Ranks
    Rank1 = 0x00000000000000FFULL,
    Rank2 = 0x000000000000FF00ULL,
    Rank3 = 0x0000000000FF0000ULL,
    Rank4 = 0x00000000FF000000ULL,
    Rank5 = 0x000000FF00000000ULL,
    Rank6 = 0x0000FF0000000000ULL,
    Rank7 = 0x00FF000000000000ULL,
    Rank8 = 0xFF00000000000000ULL,
    // Files
    FileA = 0x0101010101010101ULL,
    FileB = 0x0202020202020202ULL,
    FileC = 0x0404040404040404ULL,
    FileD = 0x0808080808080808ULL,
    FileE = 0x1010101010101010ULL,
    FileF = 0x2020202020202020ULL,
    FileG = 0x4040404040404040ULL,
    FileH = 0x8080808080808080ULL,
    // Other
    Empty = 0x0000000000000000ULL,
    Full = 0xFFFFFFFFFFFFFFFFULL,
    Light = 0x55AA55AA55AA55AAULL,
    Dark = 0xAA55AA55AA55AA55ULL,
    QueenSide = 0x0F0F0F0F0F0F0F0FULL,
    KingSide = 0xF0F0F0F0F0F0F0F0ULL,
};

class BitboardIterator {
   public:
    [[nodiscard]] constexpr BitboardIterator() noexcept = default;

    [[nodiscard]] constexpr BitboardIterator(const std::uint64_t mask) noexcept : m_mask(mask) {
    }

    [[nodiscard]] constexpr auto operator!=(const BitboardIterator rhs) const noexcept -> bool {
        return m_mask != rhs.m_mask;
    }

    [[nodiscard]] constexpr auto operator*() const noexcept -> Square {
        return Square(std::countr_zero(m_mask));
    }

    constexpr auto operator++() noexcept -> BitboardIterator & {
        m_mask &= m_mask - 1;
        return *this;
    }

   private:
    std::uint64_t m_mask = 0;
};

class Bitboard {
   public:
    using value_type = std::uint64_t;
    using const_iterator = BitboardIterator;

    [[nodiscard]] constexpr Bitboard() noexcept = default;

    [[nodiscard]] constexpr explicit Bitboard(const Square sq) noexcept : m_mask(1ULL << index(sq)) {
    }

    [[nodiscard]] constexpr explicit Bitboard(const value_type mask) noexcept : m_mask(mask) {
    }

    [[nodiscard]] static constexpr auto file_of(const Square sq) noexcept -> Bitboard {
        return Bitboard(Bitmask::FileA << file(sq));
    }

    [[nodiscard]] static constexpr auto rank_of(const Square sq) noexcept -> Bitboard {
        return Bitboard(Bitmask::Rank1 << (8 * (index(sq) / 8)));
    }

    [[nodiscard]] constexpr explicit operator bool() const noexcept {
        return m_mask != Bitmask::Empty;
    }

    [[nodiscard]] constexpr auto operator==(const Bitboard rhs) const noexcept -> bool {
        return m_mask == rhs.m_mask;
    }

    constexpr auto operator|=(const Bitboard rhs) noexcept -> Bitboard {
        m_mask |= rhs.m_mask;
        return *this;
    }

    constexpr auto operator&=(const Bitboard rhs) noexcept -> Bitboard {
        m_mask &= rhs.m_mask;
        return *this;
    }

    constexpr auto operator^=(const Bitboard rhs) noexcept -> Bitboard {
        m_mask ^= rhs.m_mask;
        return *this;
    }

    [[nodiscard]] constexpr auto operator~() const noexcept -> Bitboard {
        return Bitboard(~m_mask);
    }

    [[nodiscard]] constexpr auto operator|(const Bitboard rhs) const noexcept -> Bitboard {
        return Bitboard(m_mask | rhs.m_mask);
    }

    [[nodiscard]] constexpr auto operator&(const Bitboard rhs) const noexcept -> Bitboard {
        return Bitboard(m_mask & rhs.m_mask);
    }

    [[nodiscard]] constexpr auto operator^(const Bitboard rhs) const noexcept -> Bitboard {
        return Bitboard(m_mask ^ rhs.m_mask);
    }

    [[nodiscard]] constexpr auto is_occupied(const Square sq) const noexcept -> bool {
        return m_mask & (0x1ULL << index(sq));
    }

    [[nodiscard]] constexpr auto is_empty(const Square sq) const noexcept -> bool {
        return !is_occupied(sq);
    }

    [[nodiscard]] constexpr auto empty() const noexcept -> bool {
        return m_mask == Bitmask::Empty;
    }

    [[nodiscard]] constexpr auto lsb() const noexcept -> Square {
        return Square(std::countr_zero(m_mask));
    }

    [[nodiscard]] constexpr auto hsb() const noexcept -> Square {
        return Square(63 - std::countl_zero(m_mask));
    }

    [[nodiscard]] constexpr auto count() const noexcept -> int {
        return std::popcount(m_mask);
    }

    [[nodiscard]] constexpr auto data() const noexcept -> value_type {
        return m_mask;
    }

    [[nodiscard]] constexpr auto north() const noexcept -> Bitboard {
        return Bitboard(m_mask << 8);
    }

    [[nodiscard]] constexpr auto south() const noexcept -> Bitboard {
        return Bitboard(m_mask >> 8);
    }

    [[nodiscard]] constexpr auto east() const noexcept -> Bitboard {
        return Bitboard((m_mask << 1) & ~Bitmask::FileA);
    }

    [[nodiscard]] constexpr auto west() const noexcept -> Bitboard {
        return Bitboard((m_mask >> 1) & ~Bitmask::FileH);
    }

    [[nodiscard]] constexpr auto northeast() const noexcept -> Bitboard {
        return Bitboard((m_mask << 9) & ~Bitmask::FileA);
    }

    [[nodiscard]] constexpr auto northwest() const noexcept -> Bitboard {
        return Bitboard((m_mask << 7) & ~Bitmask::FileH);
    }

    [[nodiscard]] constexpr auto southeast() const noexcept -> Bitboard {
        return Bitboard((m_mask >> 7) & ~Bitmask::FileA);
    }

    [[nodiscard]] constexpr auto southwest() const noexcept -> Bitboard {
        return Bitboard((m_mask >> 9) & ~Bitmask::FileH);
    }

    [[nodiscard]] constexpr auto adjacent() const noexcept -> Bitboard {
        return Bitboard((((m_mask << 1) | (m_mask << 9) | (m_mask >> 7)) & ~Bitmask::FileA) |
                        (((m_mask >> 1) | (m_mask >> 9) | (m_mask << 7)) & ~Bitmask::FileH) | (m_mask << 8) |
                        (m_mask >> 8));
    }

    [[nodiscard]] constexpr auto extend_north() const noexcept -> Bitboard {
        const auto a = m_mask | m_mask << 8;
        const auto b = a | a << 16;
        const auto c = b | b << 32;
        return Bitboard(c);
    }

    [[nodiscard]] constexpr auto extend_south() const noexcept -> Bitboard {
        const auto a = m_mask | m_mask >> 8;
        const auto b = a | a >> 16;
        const auto c = b | b >> 32;
        return Bitboard(c);
    }

    [[nodiscard]] constexpr auto knight() const noexcept -> Bitboard {
        auto result = Bitboard();
        result |= Bitboard(((m_mask << 17) | (m_mask >> 15)) & ~Bitmask::FileA);
        result |= Bitboard(((m_mask << 15) | (m_mask >> 17)) & ~Bitmask::FileH);
        result |= Bitboard(((m_mask << 10) | (m_mask >> 6)) & ~(Bitmask::FileB | Bitmask::FileA));
        result |= Bitboard(((m_mask << 6) | (m_mask >> 10)) & ~(Bitmask::FileH | Bitmask::FileG));
        return result;
    }

    [[nodiscard]] constexpr auto forwards(const Colour colour) const noexcept -> Bitboard {
        if (colour == Colour::White) {
            return north();
        } else {
            return south();
        }
    }

    [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator {
        return {m_mask};
    }

    [[nodiscard]] constexpr auto end() const noexcept -> const_iterator {
        return {};
    }

    [[nodiscard]] constexpr auto operator>>(const int n) const noexcept -> Bitboard {
        return Bitboard(m_mask >> n);
    }

    [[nodiscard]] constexpr auto operator<<(const int n) const noexcept -> Bitboard {
        return Bitboard(m_mask << n);
    }

    constexpr auto clear() noexcept -> void {
        m_mask = Bitmask::Empty;
    }

   private:
    value_type m_mask = Bitmask::Empty;
};

inline auto operator<<(std::ostream &os, const Bitboard &bb) noexcept -> std::ostream & {
    for (int y = 7; y >= 0; --y) {
        for (int x = 0; x < 8; ++x) {
            const auto sq = Square(y * 8 + x);
            os << bb.is_occupied(sq);
        }
        os << '\n';
    }
    return os;
}

static_assert(sizeof(Bitboard) == sizeof(Bitboard::value_type));
// Bitboard::operator==()
static_assert(Bitboard() == Bitboard());
static_assert(Bitboard(Bitmask::Empty) == Bitboard());
static_assert(Bitboard(0x1ULL) == Bitboard(0x1ULL));
// Bitboard::operator!=()
static_assert(Bitboard() != Bitboard(0x1ULL));
static_assert(Bitboard(Bitmask::Light) != Bitboard(Bitmask::Dark));
// Bitboard::operator~()
static_assert(Bitboard(Bitmask::Full) == ~Bitboard(Bitmask::Empty));
static_assert(Bitboard(Bitmask::Light) == ~Bitboard(Bitmask::Dark));
// Bitboard::operator&()
static_assert((Bitboard(Bitmask::Full) & Bitboard(Bitmask::Light)) == Bitboard(Bitmask::Light));
static_assert((Bitboard(Bitmask::Light) & Bitboard(Bitmask::Dark)) == Bitboard(Bitmask::Empty));
// Bitboard::operator|()
static_assert((Bitboard(Bitmask::Light) | Bitboard(Bitmask::Dark)) == Bitboard(Bitmask::Full));
// Bitboard::operator^()
static_assert((Bitboard(Bitmask::Light) ^ Bitboard(Bitmask::Dark)) == Bitboard(Bitmask::Full));
static_assert((Bitboard(Bitmask::Full) ^ Bitboard(Bitmask::Full)) == Bitboard(Bitmask::Empty));
static_assert((Bitboard(Bitmask::Full) ^ Bitboard(Bitmask::Light) ^ Bitboard(Bitmask::Dark)) ==
              Bitboard(Bitmask::Empty));
// Bitboard::operator>>
static_assert(Bitboard(Bitmask::FileH) >> 1 == Bitboard(Bitmask::FileG));
static_assert(Bitboard(Bitmask::FileH) >> 2 == Bitboard(Bitmask::FileF));
static_assert(Bitboard(Bitmask::FileH) >> 3 == Bitboard(Bitmask::FileE));
static_assert(Bitboard(Bitmask::Rank3) >> 8 == Bitboard(Bitmask::Rank2));
static_assert(Bitboard(Bitmask::Rank3) >> 16 == Bitboard(Bitmask::Rank1));
static_assert(Bitboard(Bitmask::Rank3) >> 24 == Bitboard(Bitmask::Empty));
// Bitboard::operator<<
static_assert(Bitboard(Bitmask::FileA) << 1 == Bitboard(Bitmask::FileB));
static_assert(Bitboard(Bitmask::FileA) << 2 == Bitboard(Bitmask::FileC));
static_assert(Bitboard(Bitmask::FileA) << 3 == Bitboard(Bitmask::FileD));
static_assert(Bitboard(Bitmask::Rank3) << 8 == Bitboard(Bitmask::Rank4));
static_assert(Bitboard(Bitmask::Rank3) << 16 == Bitboard(Bitmask::Rank5));
static_assert(Bitboard(Bitmask::Rank8) << 8 == Bitboard(Bitmask::Empty));
// Bitboard::is_occupied()
static_assert(!Bitboard().is_occupied(Square::A1));
static_assert(Bitboard(0x1ULL).is_occupied(Square::A1));
static_assert(Bitboard(Bitmask::Light).is_occupied(Square::H1));
static_assert(Bitboard(Bitmask::Light).is_occupied(Square::A8));
static_assert(!Bitboard(Bitmask::Dark).is_occupied(Square::H1));
static_assert(!Bitboard(Bitmask::Dark).is_occupied(Square::A8));
// Bitboard::empty()
static_assert(Bitboard().empty());
static_assert(!Bitboard(Bitmask::Rank1).empty());
static_assert(!Bitboard(Bitmask::Full).empty());
// Bitboard::lsb()
static_assert(Bitboard(0x1ULL).lsb() == Square::A1);
static_assert(Bitboard(Bitmask::Rank4).lsb() == Square::A4);
static_assert(Bitboard(Bitmask::FileE).lsb() == Square::E1);
static_assert(Bitboard(Bitmask::Full).lsb() == Square::A1);
// Bitboard::hsb()
static_assert(Bitboard(0x1ULL).hsb() == Square::A1);
static_assert(Bitboard(Bitmask::Rank4).hsb() == Square::H4);
static_assert(Bitboard(Bitmask::FileE).hsb() == Square::E8);
static_assert(Bitboard(Bitmask::Full).hsb() == Square::H8);
// Bitboard::count()
static_assert(Bitboard().count() == 0);
static_assert(Bitboard(0x1ULL).count() == 1);
static_assert(Bitboard(Bitmask::Rank1).count() == 8);
static_assert(Bitboard(Bitmask::Light).count() == 32);
static_assert(Bitboard(Bitmask::Dark).count() == 32);
static_assert(Bitboard(Bitmask::Full).count() == 64);
// Bitboard::north()
static_assert(Bitboard(Bitmask::Rank1).north() == Bitboard(Bitmask::Rank2));
static_assert(Bitboard(Bitmask::Rank8).north() == Bitboard(Bitmask::Empty));
// Bitboard::south()
static_assert(Bitboard(Bitmask::Rank2).south() == Bitboard(Bitmask::Rank1));
static_assert(Bitboard(Bitmask::Rank1).south() == Bitboard(Bitmask::Empty));
// Bitboard::east()
static_assert(Bitboard(Bitmask::FileA).east() == Bitboard(Bitmask::FileB));
static_assert(Bitboard(Bitmask::FileH).east() == Bitboard(Bitmask::Empty));
// Bitboard::west()
static_assert(Bitboard(Bitmask::FileB).west() == Bitboard(Bitmask::FileA));
static_assert(Bitboard(Bitmask::FileA).west() == Bitboard(Bitmask::Empty));
// Bitboard::northeast()
static_assert(Bitboard(Bitmask::Full).northeast() == Bitboard(0xFEFEFEFEFEFEFE00ULL));
static_assert(Bitboard(Bitmask::Full).north().east() == Bitboard(0xFEFEFEFEFEFEFE00ULL));
// Bitboard::northwest()
static_assert(Bitboard(Bitmask::Full).northwest() == Bitboard(0x7F7F7F7F7F7F7F00ULL));
static_assert(Bitboard(Bitmask::Full).north().west() == Bitboard(0x7F7F7F7F7F7F7F00ULL));
// Bitboard::southeast()
static_assert(Bitboard(Bitmask::Full).southeast() == Bitboard(0x00FEFEFEFEFEFEFEULL));
static_assert(Bitboard(Bitmask::Full).south().east() == Bitboard(0x00FEFEFEFEFEFEFEULL));
// Bitboard::southwest()
static_assert(Bitboard(Bitmask::Full).southwest() == Bitboard(0x007F7F7F7F7F7F7FULL));
static_assert(Bitboard(Bitmask::Full).south().west() == Bitboard(0x007F7F7F7F7F7F7FULL));
// Bitboard::adjacent()
static_assert(Bitboard().adjacent() == Bitboard());
static_assert(Bitboard(Bitmask::Full).adjacent() == Bitboard(Bitmask::Full));
static_assert(Bitboard(Bitmask::FileA).adjacent() == (Bitboard(Bitmask::FileA) | Bitboard(Bitmask::FileB)));
static_assert(Bitboard(0x1ULL).adjacent() == Bitboard(0x302ULL));
static_assert(Bitboard(0x100000000000000ULL).adjacent() == Bitboard(0x203000000000000ULL));
static_assert(Bitboard(0x8000000000000000ULL).adjacent() == Bitboard(0x40C0000000000000ULL));
static_assert(Bitboard(0x80ULL).adjacent() == Bitboard(0xC040ULL));
static_assert(Bitboard(0x40000ULL).adjacent() == Bitboard(0xE0A0E00ULL));
// Bitboard::extend_north()
static_assert(Bitboard(Bitmask::Rank1).extend_north() == Bitboard(Bitmask::Full));
static_assert(Bitboard(Bitmask::FileA).extend_north() == Bitboard(Bitmask::FileA));
static_assert(Bitboard(Square::A1).extend_north() == Bitboard(Bitmask::FileA));
static_assert(Bitboard(Square::H1).extend_north() == Bitboard(Bitmask::FileH));
static_assert(Bitboard(0x4000000900100200ULL).extend_north() == Bitboard(0x5B1B1B1B12120200ULL));
// Bitboard::extend_south()
static_assert(Bitboard(Bitmask::Rank8).extend_south() == Bitboard(Bitmask::Full));
static_assert(Bitboard(Bitmask::FileA).extend_south() == Bitboard(Bitmask::FileA));
static_assert(Bitboard(Square::A8).extend_south() == Bitboard(Bitmask::FileA));
static_assert(Bitboard(Square::H8).extend_south() == Bitboard(Bitmask::FileH));
static_assert(Bitboard(0x4000000900100200ULL).extend_south() == Bitboard(0x4040404949595B5BULL));
// Bitboard::knight()
static_assert(Bitboard(0x8000000ULL).knight() == Bitboard(0x142200221400ULL));
static_assert(Bitboard(0x1ULL).knight() == Bitboard(0x20400ULL));
static_assert(Bitboard(0x80ULL).knight() == Bitboard(0x402000ULL));
static_assert(Bitboard(0x100000000000000ULL).knight() == Bitboard(0x4020000000000ULL));
static_assert(Bitboard(0x8000000000000000ULL).knight() == Bitboard(0x20400000000000ULL));
static_assert(Bitboard(Bitmask::Light).knight() == Bitboard(Bitmask::Dark));
// Bitboard::file_of()
static_assert(Bitboard::file_of(Square::A1) == Bitboard(Bitmask::FileA));
static_assert(Bitboard::file_of(Square::B2) == Bitboard(Bitmask::FileB));
static_assert(Bitboard::file_of(Square::C3) == Bitboard(Bitmask::FileC));
static_assert(Bitboard::file_of(Square::D4) == Bitboard(Bitmask::FileD));
static_assert(Bitboard::file_of(Square::E5) == Bitboard(Bitmask::FileE));
static_assert(Bitboard::file_of(Square::F6) == Bitboard(Bitmask::FileF));
static_assert(Bitboard::file_of(Square::G7) == Bitboard(Bitmask::FileG));
static_assert(Bitboard::file_of(Square::H8) == Bitboard(Bitmask::FileH));
// Bitboard::rank_of()
static_assert(Bitboard::rank_of(Square::A1) == Bitboard(Bitmask::Rank1));
static_assert(Bitboard::rank_of(Square::B2) == Bitboard(Bitmask::Rank2));
static_assert(Bitboard::rank_of(Square::C3) == Bitboard(Bitmask::Rank3));
static_assert(Bitboard::rank_of(Square::D4) == Bitboard(Bitmask::Rank4));
static_assert(Bitboard::rank_of(Square::E5) == Bitboard(Bitmask::Rank5));
static_assert(Bitboard::rank_of(Square::F6) == Bitboard(Bitmask::Rank6));
static_assert(Bitboard::rank_of(Square::G7) == Bitboard(Bitmask::Rank7));
static_assert(Bitboard::rank_of(Square::H8) == Bitboard(Bitmask::Rank8));
// Bitmasks
static_assert(Bitboard(Bitmask::QueenSide) == (Bitboard(Bitmask::FileA) | Bitboard(Bitmask::FileB) |
                                               Bitboard(Bitmask::FileC) | Bitboard(Bitmask::FileD)));
static_assert(Bitboard(Bitmask::QueenSide) == ~Bitboard(Bitmask::KingSide));

}  // namespace chess

#endif

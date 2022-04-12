#ifndef CHESS_MOVE_HPP
#define CHESS_MOVE_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <ostream>
#include "piece.hpp"
#include "square.hpp"

namespace chess {

enum class MoveType : int
{
    Quiet = 0,
    Double,
    Capture,
    KSC,
    QSC,
    EnPassant,
    Promo,
    PromoCapture,
};

class Move {
   public:
    [[nodiscard]] constexpr Move() noexcept = default;

    [[nodiscard]] constexpr Move(const MoveType type,
                                 const PieceType piece,
                                 const Square from,
                                 const Square to) noexcept {
        set_movetype(type);
        set_piece(piece);
        set_from(from);
        set_to(to);
        set_captured(PieceType::None);
        set_promo(PieceType::None);
    }

    [[nodiscard]] constexpr Move(const MoveType type,
                                 const PieceType piece,
                                 const Square from,
                                 const Square to,
                                 const PieceType captured) {
        set_movetype(type);
        set_piece(piece);
        set_from(from);
        set_to(to);
        set_captured(captured);
        set_promo(PieceType::None);
    }

    [[nodiscard]] constexpr Move(const MoveType type,
                                 const PieceType piece,
                                 const Square from,
                                 const Square to,
                                 const PieceType captured,
                                 const PieceType promo) {
        set_movetype(type);
        set_piece(piece);
        set_from(from);
        set_to(to);
        set_captured(captured);
        set_promo(promo);
    }

    [[nodiscard]] constexpr auto data() const noexcept -> std::uint32_t {
        return m_data;
    }

    [[nodiscard]] constexpr auto operator==(const Move &rhs) const noexcept -> bool {
        return m_data == rhs.m_data;
    }

    [[nodiscard]] constexpr auto type() const noexcept -> MoveType {
        return static_cast<MoveType>(m_data & 0b111);
    }

    [[nodiscard]] constexpr auto piece() const noexcept -> PieceType {
        return static_cast<PieceType>((m_data >> 3) & 0b111);
    }

    [[nodiscard]] constexpr auto captured() const noexcept -> PieceType {
        return static_cast<PieceType>((m_data >> 6) & 0b111);
    }

    [[nodiscard]] constexpr auto from() const noexcept -> Square {
        return static_cast<Square>((m_data >> 9) & 0b111111);
    }

    [[nodiscard]] constexpr auto to() const noexcept -> Square {
        return static_cast<Square>((m_data >> 15) & 0b111111);
    }

    [[nodiscard]] constexpr auto promo() const noexcept -> PieceType {
        return static_cast<PieceType>((m_data >> 21) & 0b111);
    }

    [[nodiscard]] operator std::string() const noexcept {
        std::string str;
        str += sq_strings[index(from())];
        str += sq_strings[index(to())];
        if (promo() == PieceType::Queen) {
            str += 'q';
        } else if (promo() == PieceType::Rook) {
            str += 'r';
        } else if (promo() == PieceType::Bishop) {
            str += 'b';
        } else if (promo() == PieceType::Knight) {
            str += 'n';
        }
        return str;
    }

   private:
    constexpr auto set_movetype(const MoveType mt) noexcept -> void {
        m_data |= static_cast<int>(mt) & 0b111;
    }

    constexpr auto set_piece(const PieceType piece) noexcept -> void {
        m_data |= (static_cast<int>(piece) & 0b111) << 3;
    }

    constexpr auto set_captured(const PieceType piece) noexcept -> void {
        m_data |= (static_cast<int>(piece) & 0b111) << 6;
    }

    constexpr auto set_from(const Square from) noexcept -> void {
        m_data |= (static_cast<int>(from) & 0b111111) << 9;
    }

    constexpr auto set_to(const Square to) noexcept -> void {
        m_data |= (static_cast<int>(to) & 0b111111) << 15;
    }

    constexpr auto set_promo(const PieceType promo) noexcept -> void {
        m_data |= (static_cast<int>(promo) & 0b111) << 21;
    }

    std::uint32_t m_data = 0;
};

inline auto operator<<(std::ostream &os, const Move &move) noexcept -> std::ostream & {
    os << static_cast<std::string>(move);
    return os;
}

static_assert(sizeof(Move) == sizeof(std::uint32_t));
// Move::operator==()
static_assert(Move(MoveType::Double, PieceType::Pawn, Square::E2, Square::E4) ==
              Move(MoveType::Double, PieceType::Pawn, Square::E2, Square::E4));
// Move::operator!=()
static_assert(Move(MoveType::Double, PieceType::Pawn, Square::E2, Square::E4) !=
              Move(MoveType::Double, PieceType::Pawn, Square::E2, Square::E5));
// Move::type()
static_assert(Move(MoveType::Quiet, PieceType::Pawn, Square::E2, Square::E4).type() == MoveType::Quiet);
static_assert(Move(MoveType::Double, PieceType::Pawn, Square::E2, Square::E4).type() == MoveType::Double);
static_assert(Move(MoveType::PromoCapture, PieceType::Pawn, Square::E7, Square::F8, PieceType::Rook, PieceType::Queen)
                  .type() == MoveType::PromoCapture);
// Move::to()
static_assert(Move(MoveType::Double, PieceType::Pawn, Square::E2, Square::E4).to() == Square::E4);
static_assert(Move(MoveType::Double, PieceType::Pawn, Square::E2, Square::E4).to() != Square::E2);
// Move::from()
static_assert(Move(MoveType::Double, PieceType::Pawn, Square::E2, Square::E4).from() == Square::E2);
static_assert(Move(MoveType::Double, PieceType::Pawn, Square::E2, Square::E4).from() != Square::E4);
// Move::promo()
static_assert(Move(MoveType::Quiet, PieceType::Pawn, Square::E7, Square::E8).promo() == PieceType::None);
static_assert(Move(MoveType::Promo, PieceType::Pawn, Square::E7, Square::E8, PieceType::None, PieceType::Knight)
                  .promo() == PieceType::Knight);
static_assert(Move(MoveType::Promo, PieceType::Pawn, Square::E7, Square::E8, PieceType::None, PieceType::Bishop)
                  .promo() == PieceType::Bishop);
static_assert(Move(MoveType::Promo, PieceType::Pawn, Square::E7, Square::E8, PieceType::None, PieceType::Rook)
                  .promo() == PieceType::Rook);
static_assert(Move(MoveType::Promo, PieceType::Pawn, Square::E7, Square::E8, PieceType::None, PieceType::Queen)
                  .promo() == PieceType::Queen);

}  // namespace chess

template <>
struct std::hash<chess::Move> {
    [[nodiscard]] auto operator()(const chess::Move &move) const -> std::size_t {
        return move.data();
    }
};

#endif

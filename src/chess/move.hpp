#ifndef LIBCHESS_MOVE_HPP
#define LIBCHESS_MOVE_HPP

#include <cassert>
#include <cstdint>
#include <ostream>
#include "../assert.hpp"
#include "piece.hpp"
#include "square.hpp"

namespace chess {

/*  Int packing:
 *  +6  6 - From
 *  +6 12 - To
 *  +3 15 - Type
 *  +3 18 - Piece
 *  +3 21 - Captured Piece
 *  +3 24 - Promotion Piece
 */

enum MoveType : int
{
    Normal = 0,
    Capture,
    Double,
    enpassant,
    ksc,
    qsc,
    promo,
    promo_capture
};

class Move {
   public:
    Move() : data_{0} {
    }

    Move(const MoveType _t,
         const Square _fr,
         const Square _to,
         const Piece _piece,
         const Piece _cap = Piece::None,
         const Piece _promotion = Piece::None)
        : data_{} {
        data_ = static_cast<unsigned int>(_fr);
        data_ |= static_cast<unsigned int>(_to) << 6;
        data_ |= static_cast<unsigned int>(_t) << 12;
        data_ |= static_cast<unsigned int>(_piece) << 15;
        data_ |= static_cast<unsigned int>(_cap) << 18;
        data_ |= static_cast<unsigned int>(_promotion) << 21;

#ifndef NDEBUG
        UCI_ASSERT(piece() != Piece::None);
        UCI_ASSERT(to() != from());
        UCI_ASSERT(_t == type());
        UCI_ASSERT(_fr == from());
        UCI_ASSERT(_to == to());
        UCI_ASSERT(_piece == piece());
        UCI_ASSERT(_cap == captured());
        UCI_ASSERT(_promotion == promotion());

        switch (type()) {
            case MoveType::Normal:
                UCI_ASSERT(captured() == Piece::None);
                UCI_ASSERT(promotion() == Piece::None);
                break;
            case MoveType::Capture:
                UCI_ASSERT(captured() != Piece::None);
                UCI_ASSERT(captured() != Piece::King);
                UCI_ASSERT(promotion() == Piece::None);
                break;
            case MoveType::Double:
                UCI_ASSERT(piece() == Piece::Pawn);
                UCI_ASSERT(captured() == Piece::None);
                UCI_ASSERT(promotion() == Piece::None);
                break;
            case MoveType::enpassant:
                UCI_ASSERT(piece() == Piece::Pawn);
                UCI_ASSERT(captured() == Piece::Pawn);
                UCI_ASSERT(promotion() == Piece::None);
                break;
            case MoveType::ksc:
                UCI_ASSERT(piece() == Piece::King);
                UCI_ASSERT(captured() == Piece::None);
                UCI_ASSERT(promotion() == Piece::None);
                break;
            case MoveType::qsc:
                UCI_ASSERT(piece() == Piece::King);
                UCI_ASSERT(captured() == Piece::None);
                UCI_ASSERT(promotion() == Piece::None);
                break;
            case MoveType::promo:
                UCI_ASSERT(piece() == Piece::Pawn);
                UCI_ASSERT(captured() == Piece::None);
                UCI_ASSERT(promotion() != Piece::None);
                UCI_ASSERT(promotion() != Piece::Pawn);
                UCI_ASSERT(promotion() != Piece::King);
                break;
            case MoveType::promo_capture:
                UCI_ASSERT(piece() == Piece::Pawn);
                UCI_ASSERT(captured() != Piece::None);
                UCI_ASSERT(captured() != Piece::Pawn);
                UCI_ASSERT(captured() != Piece::King);
                UCI_ASSERT(promotion() != Piece::None);
                UCI_ASSERT(promotion() != Piece::Pawn);
                UCI_ASSERT(promotion() != Piece::King);
                break;
            default:
                abort();
        }
#endif
    }

    [[nodiscard]] constexpr Square from() const noexcept {
        return Square(data_ & 0x3F);
    }

    [[nodiscard]] constexpr Square to() const noexcept {
        return Square((data_ >> 6) & 0x3F);
    }

    [[nodiscard]] constexpr MoveType type() const noexcept {
        return MoveType((data_ >> 12) & 0x7);
    }

    [[nodiscard]] constexpr Piece piece() const noexcept {
        return static_cast<Piece>((data_ >> 15) & 0x7);
    }

    [[nodiscard]] constexpr Piece captured() const noexcept {
        return static_cast<Piece>((data_ >> 18) & 0x7);
    }

    [[nodiscard]] constexpr Piece promotion() const noexcept {
        return static_cast<Piece>((data_ >> 21) & 0x7);
    }

    [[nodiscard]] constexpr operator bool() const noexcept {
        return data_;
    }

    [[nodiscard]] operator std::string() const noexcept {
        std::string str;
        str += static_cast<std::string>(from());
        str += static_cast<std::string>(to());
        if (promotion() != Piece::None) {
            const char asd[] = {'n', 'b', 'r', 'q'};
            str += asd[static_cast<int>(promotion()) - 1];
        }
        return str;
    }

    [[nodiscard]] constexpr bool operator==(const Move &rhs) const noexcept {
        return data_ == rhs.data_;
    }

    [[nodiscard]] constexpr bool operator!=(const Move &rhs) const noexcept {
        return data_ != rhs.data_;
    }

    [[nodiscard]] constexpr bool is_capturing() const noexcept {
        return type() == MoveType::Capture || type() == MoveType::promo_capture || type() == MoveType::enpassant;
    }

    [[nodiscard]] constexpr bool is_promoting() const noexcept {
        return type() == MoveType::promo || type() == MoveType::promo_capture;
    }

   private:
    std::uint32_t data_;
};

inline std::ostream &operator<<(std::ostream &os, const Move &move) noexcept {
    os << static_cast<std::string>(move);
    return os;
}

static_assert(sizeof(Move) == sizeof(std::uint32_t));

}  // namespace chess

#endif

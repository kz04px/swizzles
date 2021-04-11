#include "../assert.hpp"
#include "position.hpp"

namespace chess {

void Position::makemove(const Move &move) noexcept {
    const auto us = turn();
    const auto them = !turn();
    const auto to = move.to();
    const auto from = move.from();
    const auto piece = move.piece();
    const auto captured = move.captured();
    const auto promo = move.promotion();
    const auto hash_old = hash_;
    const auto ep_old = ep_;
    const auto halfmove_clock_old = halfmove_clock_;

    UCI_ASSERT(to != from);
    UCI_ASSERT(piece != Piece::None);
    UCI_ASSERT(captured != Piece::King);
    UCI_ASSERT(promo != Piece::Pawn);
    UCI_ASSERT(promo != Piece::King);
    UCI_ASSERT(piece_on(move.from()) == piece);

    // Remove piece
    colours_[static_cast<int>(us)] ^= move.from();
    pieces_[static_cast<int>(piece)] ^= move.from();

    // Add piece
    colours_[static_cast<int>(us)] ^= move.to();
    pieces_[static_cast<int>(piece)] ^= move.to();

    // Fullmoves
    fullmove_clock_ += us == Side::Black;

    hash_ ^= zobrist::turn_key();
    hash_ ^= zobrist::piece_key(piece, us, move.from());
    hash_ ^= zobrist::piece_key(piece, us, move.to());
    if (ep_ != 0xFF) {
        hash_ ^= zobrist::ep_key(ep_);
    }

    // Remove ep
    ep_ = 0xFF;

    // Increment halfmove clock
    halfmove_clock_++;

    switch (move.type()) {
        case MoveType::Normal:
            UCI_ASSERT(captured == Piece::None);
            UCI_ASSERT(promo == Piece::None);

            if (piece == Piece::Pawn) {
                halfmove_clock_ = 0;
            }
            break;
        case MoveType::Capture:
            UCI_ASSERT(captured != Piece::None);
            UCI_ASSERT(promo == Piece::None);

            halfmove_clock_ = 0;

            hash_ ^= zobrist::piece_key(captured, them, move.to());

            // Remove the captured piece
            pieces_[static_cast<int>(captured)] ^= move.to();
            colours_[static_cast<int>(them)] ^= move.to();
            break;
        case MoveType::Double:
            UCI_ASSERT(piece == Piece::Pawn);
            UCI_ASSERT(captured == Piece::None);
            UCI_ASSERT(promo == Piece::None);
            UCI_ASSERT(to.file() == from.file());
            UCI_ASSERT((us == Side::White && move.to().rank() == 3) || (us == Side::Black && move.to().rank() == 4));
            UCI_ASSERT((us == Side::White && move.from().rank() == 1) ||
                       (us == Side::Black && move.from().rank() == 6));

            halfmove_clock_ = 0;
            ep_ = to.file();

            hash_ ^= zobrist::ep_key(ep_);

            break;
        case MoveType::enpassant:
            UCI_ASSERT(piece == Piece::Pawn);
            UCI_ASSERT(captured == Piece::Pawn);
            UCI_ASSERT(promo == Piece::None);
            UCI_ASSERT(to.file() == ep_old);
            UCI_ASSERT((us == Side::White && move.to().rank() == 5) || (us == Side::Black && move.to().rank() == 2));
            UCI_ASSERT((us == Side::White && move.from().rank() == 4) ||
                       (us == Side::Black && move.from().rank() == 3));
            UCI_ASSERT(to.file() - from.file() == 1 || from.file() - to.file() == 1);

            halfmove_clock_ = 0;

            // Remove the captured pawn
            if (turn() == Side::White) {
                pieces_[static_cast<int>(Piece::Pawn)] ^= move.to().south();
                colours_[static_cast<int>(Side::Black)] ^= move.to().south();
                hash_ ^= zobrist::piece_key(Piece::Pawn, them, move.to().south());
            } else {
                pieces_[static_cast<int>(Piece::Pawn)] ^= move.to().north();
                colours_[static_cast<int>(Side::White)] ^= move.to().north();
                hash_ ^= zobrist::piece_key(Piece::Pawn, them, move.to().north());
            }
            break;
        case MoveType::ksc:
            UCI_ASSERT(piece == Piece::King);
            UCI_ASSERT(captured == Piece::None);
            UCI_ASSERT(promo == Piece::None);
            UCI_ASSERT(can_castle(us, MoveType::ksc));
            UCI_ASSERT(us == Side::White ? move.from() == squares::E1 : move.from() == squares::E8);
            UCI_ASSERT(us == Side::White ? move.to() == squares::G1 : move.to() == squares::G8);
            UCI_ASSERT(us == Side::White ? piece_on(squares::E1) == Piece::None : piece_on(squares::E8) == Piece::None);
            UCI_ASSERT(us == Side::White ? piece_on(squares::F1) == Piece::None : piece_on(squares::F8) == Piece::None);
            UCI_ASSERT(us == Side::White ? piece_on(squares::G1) == Piece::King : piece_on(squares::G8) == Piece::King);
            UCI_ASSERT(us == Side::White ? piece_on(squares::H1) == Piece::Rook : piece_on(squares::H8) == Piece::Rook);
            UCI_ASSERT(us == Side::White ? !square_attacked(squares::E1, them) : !square_attacked(squares::E8, them));
            UCI_ASSERT(us == Side::White ? !square_attacked(squares::F1, them) : !square_attacked(squares::F8, them));
            UCI_ASSERT(us == Side::White ? !square_attacked(squares::G1, them) : !square_attacked(squares::G8, them));

            hash_ ^= zobrist::piece_key(Piece::Rook, us, ksc_rook_fr[static_cast<int>(us)]);
            hash_ ^= zobrist::piece_key(Piece::Rook, us, ksc_rook_to[static_cast<int>(us)]);

            // Remove the rook
            colours_[static_cast<int>(us)] ^= ksc_rook_fr[static_cast<int>(us)];
            pieces_[static_cast<int>(Piece::Rook)] ^= ksc_rook_fr[static_cast<int>(us)];
            // Add the rook
            colours_[static_cast<int>(us)] ^= ksc_rook_to[static_cast<int>(us)];
            pieces_[static_cast<int>(Piece::Rook)] ^= ksc_rook_to[static_cast<int>(us)];
            break;
        case MoveType::qsc:
            UCI_ASSERT(piece == Piece::King);
            UCI_ASSERT(captured == Piece::None);
            UCI_ASSERT(promo == Piece::None);
            UCI_ASSERT(can_castle(us, MoveType::qsc));
            UCI_ASSERT(us == Side::White ? move.from() == squares::E1 : move.from() == squares::E8);
            UCI_ASSERT(us == Side::White ? move.to() == squares::C1 : move.to() == squares::C8);
            UCI_ASSERT(us == Side::White ? piece_on(squares::E1) == Piece::None : piece_on(squares::E8) == Piece::None);
            UCI_ASSERT(us == Side::White ? piece_on(squares::D1) == Piece::None : piece_on(squares::D8) == Piece::None);
            UCI_ASSERT(us == Side::White ? piece_on(squares::C1) == Piece::King : piece_on(squares::C8) == Piece::King);
            UCI_ASSERT(us == Side::White ? piece_on(squares::B1) == Piece::None : piece_on(squares::B8) == Piece::None);
            UCI_ASSERT(us == Side::White ? piece_on(squares::A1) == Piece::Rook : piece_on(squares::A8) == Piece::Rook);
            UCI_ASSERT(us == Side::White ? !square_attacked(squares::E1, them) : !square_attacked(squares::E8, them));
            UCI_ASSERT(us == Side::White ? !square_attacked(squares::D1, them) : !square_attacked(squares::D8, them));
            UCI_ASSERT(us == Side::White ? !square_attacked(squares::C1, them) : !square_attacked(squares::C8, them));

            hash_ ^= zobrist::piece_key(Piece::Rook, us, qsc_rook_fr[static_cast<int>(us)]);
            hash_ ^= zobrist::piece_key(Piece::Rook, us, qsc_rook_to[static_cast<int>(us)]);

            // Remove the rook
            colours_[static_cast<int>(us)] ^= qsc_rook_fr[static_cast<int>(us)];
            pieces_[static_cast<int>(Piece::Rook)] ^= qsc_rook_fr[static_cast<int>(us)];
            // Add the rook
            colours_[static_cast<int>(us)] ^= qsc_rook_to[static_cast<int>(us)];
            pieces_[static_cast<int>(Piece::Rook)] ^= qsc_rook_to[static_cast<int>(us)];
            break;
        case MoveType::promo:
            UCI_ASSERT(piece == Piece::Pawn);
            UCI_ASSERT(captured == Piece::None);
            UCI_ASSERT(promo != Piece::None);
            UCI_ASSERT(move.to().file() == move.from().file());
            UCI_ASSERT((us == Side::White && move.to().rank() == 7) || (us == Side::Black && move.to().rank() == 0));
            UCI_ASSERT((us == Side::White && move.from().rank() == 6) ||
                       (us == Side::Black && move.from().rank() == 1));

            halfmove_clock_ = 0;

            hash_ ^= zobrist::piece_key(Piece::Pawn, us, move.to());
            hash_ ^= zobrist::piece_key(promo, us, move.to());

            // Replace pawn with piece
            pieces_[static_cast<int>(Piece::Pawn)] ^= move.to();
            pieces_[static_cast<int>(promo)] ^= move.to();
            break;
        case MoveType::promo_capture:
            UCI_ASSERT(piece == Piece::Pawn);
            UCI_ASSERT(captured != Piece::None);
            UCI_ASSERT(promo != Piece::None);
            UCI_ASSERT(move.to().file() != move.from().file());
            UCI_ASSERT((us == Side::White && move.to().rank() == 7) || (us == Side::Black && move.to().rank() == 0));
            UCI_ASSERT((us == Side::White && move.from().rank() == 6) ||
                       (us == Side::Black && move.from().rank() == 1));

            halfmove_clock_ = 0;

            hash_ ^= zobrist::piece_key(captured, them, move.to());
            hash_ ^= zobrist::piece_key(Piece::Pawn, us, move.to());
            hash_ ^= zobrist::piece_key(promo, us, move.to());

            // Replace pawn with piece
            pieces_[static_cast<int>(Piece::Pawn)] ^= move.to();
            pieces_[static_cast<int>(promo)] ^= move.to();
            // Remove the captured piece
            pieces_[static_cast<int>(captured)] ^= move.to();
            colours_[static_cast<int>(them)] ^= move.to();
            break;
        default:
            abort();
    }

    const bool castling_old[] = {
        castling_[0],
        castling_[1],
        castling_[2],
        castling_[3],
    };

    // Add to history
    history_.push_back(
        meh{hash_old, move, ep_old, halfmove_clock_old, {castling_[0], castling_[1], castling_[2], castling_[3]}});

    // Castling permissions
    castling_[usKSC] &= !(to == squares::H1 || from == squares::E1 || from == squares::H1);
    castling_[usQSC] &= !(to == squares::A1 || from == squares::E1 || from == squares::A1);
    castling_[themKSC] &= !(to == squares::H8 || from == squares::E8 || from == squares::H8);
    castling_[themQSC] &= !(to == squares::A8 || from == squares::E8 || from == squares::A8);

    if (castling_[usKSC] != castling_old[usKSC]) {
        hash_ ^= zobrist::castling_key(usKSC);
    }
    if (castling_[usQSC] != castling_old[usQSC]) {
        hash_ ^= zobrist::castling_key(usQSC);
    }
    if (castling_[themKSC] != castling_old[themKSC]) {
        hash_ ^= zobrist::castling_key(themKSC);
    }
    if (castling_[themQSC] != castling_old[themQSC]) {
        hash_ ^= zobrist::castling_key(themQSC);
    }

    // Swap sides
    to_move_ = !to_move_;

    UCI_ASSERT(valid());
}

}  // namespace chess

#include "../assert.hpp"
#include "position.hpp"

namespace chess {

void Position::undomove() noexcept {
    // Swap sides
    to_move_ = !to_move_;

    const auto &move = history_.back().move;
    const auto us = turn();
    const auto them = !turn();
    const auto piece = move.piece();
    const auto captured = move.captured();
    const auto promo = move.promotion();

    // En passant
    ep_ = history_.back().ep;

    // Halfmoves
    halfmove_clock_ = history_.back().halfmove_clock;

    // Fullmoves
    fullmove_clock_ -= us == Side::Black;

    // Castling
    castling_[0] = history_.back().castling[0];
    castling_[1] = history_.back().castling[1];
    castling_[2] = history_.back().castling[2];
    castling_[3] = history_.back().castling[3];

    hash_ = history_.back().hash;

    // Remove piece
    colours_[static_cast<int>(us)] ^= move.to();
    pieces_[static_cast<int>(piece)] ^= move.to();

    // Add piece
    colours_[static_cast<int>(us)] ^= move.from();
    pieces_[static_cast<int>(piece)] ^= move.from();

    switch (move.type()) {
        case MoveType::Normal:
            break;
        case MoveType::Double:
            break;
        case MoveType::Capture:
            colours_[static_cast<int>(them)] ^= move.to();
            pieces_[static_cast<int>(captured)] ^= move.to();
            break;
        case MoveType::enpassant:
            // Replace the captured pawn
            if (turn() == Side::White) {
                pieces_[static_cast<int>(Piece::Pawn)] ^= move.to().south();
                colours_[static_cast<int>(Side::Black)] ^= move.to().south();
            } else {
                pieces_[static_cast<int>(Piece::Pawn)] ^= move.to().north();
                colours_[static_cast<int>(Side::White)] ^= move.to().north();
            }
            break;
        case MoveType::ksc:
            // Remove the rook
            colours_[static_cast<int>(us)] ^= ksc_rook_fr[static_cast<int>(us)];
            pieces_[static_cast<int>(Piece::Rook)] ^= ksc_rook_fr[static_cast<int>(us)];
            // Add the rook
            colours_[static_cast<int>(us)] ^= ksc_rook_to[static_cast<int>(us)];
            pieces_[static_cast<int>(Piece::Rook)] ^= ksc_rook_to[static_cast<int>(us)];
            break;
        case MoveType::qsc:
            // Remove the rook
            colours_[static_cast<int>(us)] ^= qsc_rook_fr[static_cast<int>(us)];
            pieces_[static_cast<int>(Piece::Rook)] ^= qsc_rook_fr[static_cast<int>(us)];
            // Add the rook
            colours_[static_cast<int>(us)] ^= qsc_rook_to[static_cast<int>(us)];
            pieces_[static_cast<int>(Piece::Rook)] ^= qsc_rook_to[static_cast<int>(us)];
            break;
        case MoveType::promo:
            // Replace piece with pawn
            pieces_[static_cast<int>(Piece::Pawn)] ^= move.to();
            pieces_[static_cast<int>(promo)] ^= move.to();
            break;
        case MoveType::promo_capture:
            // Replace pawn with piece
            pieces_[static_cast<int>(Piece::Pawn)] ^= move.to();
            pieces_[static_cast<int>(promo)] ^= move.to();
            // Replace the captured piece
            pieces_[static_cast<int>(captured)] ^= move.to();
            colours_[static_cast<int>(them)] ^= move.to();
            break;
        default:
            break;
    }

    // Remove from history
    history_.pop_back();

    UCI_ASSERT(valid());
}

}  // namespace chess

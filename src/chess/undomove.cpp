#include "position.hpp"

namespace chess {

auto Position::undomove() noexcept -> void {
    m_turn = !m_turn;

    const auto move = m_history.back().move;
    const auto us = m_turn;
    const auto them = !us;
    const auto piece = move.piece();

    switch (move.type()) {
        case MoveType::Quiet:
            m_colour[index(us)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            m_piece[index(piece)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            break;
        case MoveType::Double:
            m_colour[index(us)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            m_piece[index(PieceType::Pawn)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            break;
        case MoveType::Capture:
            // Move our piece
            m_colour[index(us)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            m_piece[index(piece)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            // Replace the captured piece
            m_colour[index(them)] ^= Bitboard(move.to());
            m_piece[index(move.captured())] ^= Bitboard(move.to());
            break;
        case MoveType::KSC:
            if (us == Colour::White) {
                // Rook
                m_colour[index(Colour::White)] ^= Bitboard(move.to()) ^ Bitboard(Square::F1);
                m_piece[index(PieceType::Rook)] ^= Bitboard(move.to()) ^ Bitboard(Square::F1);
                // King
                m_colour[index(Colour::White)] ^= Bitboard(move.from()) ^ Bitboard(Square::G1);
                m_piece[index(PieceType::King)] ^= Bitboard(move.from()) ^ Bitboard(Square::G1);
            } else {
                // Rook
                m_colour[index(Colour::Black)] ^= Bitboard(move.to()) ^ Bitboard(Square::F8);
                m_piece[index(PieceType::Rook)] ^= Bitboard(move.to()) ^ Bitboard(Square::F8);
                // King
                m_colour[index(Colour::Black)] ^= Bitboard(move.from()) ^ Bitboard(Square::G8);
                m_piece[index(PieceType::King)] ^= Bitboard(move.from()) ^ Bitboard(Square::G8);
            }
            break;
        case MoveType::QSC:
            if (us == Colour::White) {
                // Rook
                m_colour[index(Colour::White)] ^= Bitboard(move.to()) ^ Bitboard(Square::D1);
                m_piece[index(PieceType::Rook)] ^= Bitboard(move.to()) ^ Bitboard(Square::D1);
                // King
                m_colour[index(Colour::White)] ^= Bitboard(move.from()) ^ Bitboard(Square::C1);
                m_piece[index(PieceType::King)] ^= Bitboard(move.from()) ^ Bitboard(Square::C1);
            } else {
                // Rook
                m_colour[index(Colour::Black)] ^= Bitboard(move.to()) ^ Bitboard(Square::D8);
                m_piece[index(PieceType::Rook)] ^= Bitboard(move.to()) ^ Bitboard(Square::D8);
                // King
                m_colour[index(Colour::Black)] ^= Bitboard(move.from()) ^ Bitboard(Square::C8);
                m_piece[index(PieceType::King)] ^= Bitboard(move.from()) ^ Bitboard(Square::C8);
            }
            break;
        case MoveType::EnPassant:
            // Remove their pawn
            if (us == Colour::White) {
                const auto sq = static_cast<Square>(index(move.to()) - 8);
                m_colour[index(Colour::Black)] ^= Bitboard(sq);
                m_piece[index(PieceType::Pawn)] ^= Bitboard(sq);
            } else {
                const auto sq = static_cast<Square>(index(move.to()) + 8);
                m_colour[index(Colour::White)] ^= Bitboard(sq);
                m_piece[index(PieceType::Pawn)] ^= Bitboard(sq);
            }

            // Move our pawn
            m_colour[index(us)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            m_piece[index(PieceType::Pawn)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            break;
        case MoveType::Promo:
            m_colour[index(us)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            m_piece[index(PieceType::Pawn)] ^= Bitboard(move.from());
            m_piece[index(move.promo())] ^= Bitboard(move.to());
            break;
        case MoveType::PromoCapture:
            m_colour[index(us)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            m_colour[index(them)] ^= Bitboard(move.to());
            m_piece[index(PieceType::Pawn)] ^= Bitboard(move.from());
            m_piece[index(move.promo())] ^= Bitboard(move.to());
            m_piece[index(move.captured())] ^= Bitboard(move.to());
            break;
    }

    m_fullmoves -= (m_turn == Colour::Black);
    restore_history();
}

}  // namespace chess

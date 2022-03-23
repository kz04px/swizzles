#include "position.hpp"
#include "zobrist.hpp"

namespace chess {

[[nodiscard]] auto Position::predict_hash(const Move move) const noexcept -> zobrist::hash_type {
    zobrist::hash_type hash = m_hash;

    const auto us = m_turn;
    const auto them = !us;
    const auto piece = move.piece();
    int castling = m_castling;

    hash ^= zobrist::turn_key();
    if (m_enpassant != Square::None) {
        hash ^= zobrist::ep_key(m_enpassant);
    }
    hash ^= zobrist::castling_key(castling);

    switch (move.type()) {
        case MoveType::Quiet:
            hash ^= zobrist::piece_key(piece, us, move.to());
            hash ^= zobrist::piece_key(piece, us, move.from());
            break;
        case MoveType::Double:
            hash ^= zobrist::piece_key(piece, us, move.to());
            hash ^= zobrist::piece_key(piece, us, move.from());
            hash ^= zobrist::ep_key(static_cast<Square>((index(move.to()) + index(move.from())) / 2));
            break;
        case MoveType::Capture:
            hash ^= zobrist::piece_key(move.captured(), them, move.to());
            hash ^= zobrist::piece_key(piece, us, move.to());
            hash ^= zobrist::piece_key(piece, us, move.from());
            break;
        case MoveType::KSC:
            if (us == Colour::White) {
                hash ^= zobrist::piece_key(PieceType::Rook, us, move.to());
                hash ^= zobrist::piece_key(PieceType::King, us, move.from());
                hash ^= zobrist::piece_key(PieceType::Rook, us, Square::F1);
                hash ^= zobrist::piece_key(PieceType::King, us, Square::G1);
            } else {
                hash ^= zobrist::piece_key(PieceType::Rook, us, move.to());
                hash ^= zobrist::piece_key(PieceType::King, us, move.from());
                hash ^= zobrist::piece_key(PieceType::Rook, us, Square::F8);
                hash ^= zobrist::piece_key(PieceType::King, us, Square::G8);
            }
            break;
        case MoveType::QSC:
            if (us == Colour::White) {
                hash ^= zobrist::piece_key(PieceType::Rook, us, move.to());
                hash ^= zobrist::piece_key(PieceType::King, us, move.from());
                hash ^= zobrist::piece_key(PieceType::Rook, us, Square::D1);
                hash ^= zobrist::piece_key(PieceType::King, us, Square::C1);
            } else {
                hash ^= zobrist::piece_key(PieceType::Rook, us, move.to());
                hash ^= zobrist::piece_key(PieceType::King, us, move.from());
                hash ^= zobrist::piece_key(PieceType::Rook, us, Square::D8);
                hash ^= zobrist::piece_key(PieceType::King, us, Square::C8);
            }
            break;
        case MoveType::EnPassant:
            if (us == Colour::White) {
                hash ^= zobrist::piece_key(PieceType::Pawn, them, static_cast<Square>(index(move.to()) - 8));
            } else {
                hash ^= zobrist::piece_key(PieceType::Pawn, them, static_cast<Square>(index(move.to()) + 8));
            }
            hash ^= zobrist::piece_key(PieceType::Pawn, us, move.to());
            hash ^= zobrist::piece_key(PieceType::Pawn, us, move.from());
            break;
        case MoveType::Promo:
            hash ^= zobrist::piece_key(move.promo(), us, move.to());
            hash ^= zobrist::piece_key(PieceType::Pawn, us, move.from());
            break;
        case MoveType::PromoCapture:
            hash ^= zobrist::piece_key(move.captured(), them, move.to());
            hash ^= zobrist::piece_key(move.promo(), us, move.to());
            hash ^= zobrist::piece_key(PieceType::Pawn, us, move.from());
            break;
    }

    if (move.to() == m_castle_rooks[0] || move.from() == m_castle_rooks[0] ||
        (us == Colour::White && piece == PieceType::King)) {
        castling &= ~(1 << static_cast<int>(CastleType::WhiteKingSide));
    }

    if (move.to() == m_castle_rooks[1] || move.from() == m_castle_rooks[1] ||
        (us == Colour::White && piece == PieceType::King)) {
        castling &= ~(1 << static_cast<int>(CastleType::WhiteQueenSide));
    }

    if (move.to() == m_castle_rooks[2] || move.from() == m_castle_rooks[2] ||
        (us == Colour::Black && piece == PieceType::King)) {
        castling &= ~(1 << static_cast<int>(CastleType::BlackKingSide));
    }

    if (move.to() == m_castle_rooks[3] || move.from() == m_castle_rooks[3] ||
        (us == Colour::Black && piece == PieceType::King)) {
        castling &= ~(1 << static_cast<int>(CastleType::BlackQueenSide));
    }

    hash ^= zobrist::castling_key(castling);

    return hash;
}

}  // namespace chess

#include "position.hpp"
#include "zobrist.hpp"

namespace chess {

template <bool update_hash>
auto Position::makemove(const Move &move) noexcept -> void {
    push_history(move);

    const auto us = m_turn;
    const auto them = !us;
    const auto piece = move.piece();

    if constexpr (update_hash) {
        m_hash ^= zobrist::turn_key();
        if (m_enpassant != Square::None) {
            m_hash ^= zobrist::ep_key(m_enpassant);
        }
        m_hash ^= zobrist::castling_key(m_castling);
    }

    m_halfmoves++;
    m_enpassant = Square::None;

    switch (move.type()) {
        case MoveType::Quiet:
            m_halfmoves *= (piece != PieceType::Pawn);
            m_colour[index(us)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            m_piece[index(piece)] ^= Bitboard(move.from()) ^ Bitboard(move.to());

            if constexpr (update_hash) {
                m_hash ^= zobrist::piece_key(piece, us, move.to());
                m_hash ^= zobrist::piece_key(piece, us, move.from());
            }
            break;
        case MoveType::Double:
            m_halfmoves = 0;
            m_colour[index(us)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            m_piece[index(PieceType::Pawn)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            m_enpassant = static_cast<Square>((index(move.to()) + index(move.from())) / 2);

            if constexpr (update_hash) {
                m_hash ^= zobrist::piece_key(PieceType::Pawn, us, move.to());
                m_hash ^= zobrist::piece_key(PieceType::Pawn, us, move.from());
                m_hash ^= zobrist::ep_key(m_enpassant);
            }
            break;
        case MoveType::Capture:
            m_halfmoves = 0;
            // Move our piece
            m_colour[index(us)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            m_piece[index(piece)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            // Remove their piece
            m_colour[index(them)] ^= Bitboard(move.to());
            m_piece[index(move.captured())] ^= Bitboard(move.to());

            if constexpr (update_hash) {
                m_hash ^= zobrist::piece_key(move.captured(), them, move.to());
                m_hash ^= zobrist::piece_key(piece, us, move.to());
                m_hash ^= zobrist::piece_key(piece, us, move.from());
            }
            break;
        case MoveType::KSC:
            if (us == Colour::White) {
                // Rook
                m_colour[index(Colour::White)] ^= Bitboard(move.to()) ^ Bitboard(Square::F1);
                m_piece[index(PieceType::Rook)] ^= Bitboard(move.to()) ^ Bitboard(Square::F1);
                // King
                m_colour[index(Colour::White)] ^= Bitboard(move.from()) ^ Bitboard(Square::G1);
                m_piece[index(PieceType::King)] ^= Bitboard(move.from()) ^ Bitboard(Square::G1);

                if constexpr (update_hash) {
                    m_hash ^= zobrist::piece_key(PieceType::Rook, Colour::White, move.to());
                    m_hash ^= zobrist::piece_key(PieceType::King, Colour::White, move.from());
                    m_hash ^= zobrist::piece_key(PieceType::Rook, Colour::White, Square::F1);
                    m_hash ^= zobrist::piece_key(PieceType::King, Colour::White, Square::G1);
                }
            } else {
                // Rook
                m_colour[index(Colour::Black)] ^= Bitboard(move.to()) ^ Bitboard(Square::F8);
                m_piece[index(PieceType::Rook)] ^= Bitboard(move.to()) ^ Bitboard(Square::F8);
                // King
                m_colour[index(Colour::Black)] ^= Bitboard(move.from()) ^ Bitboard(Square::G8);
                m_piece[index(PieceType::King)] ^= Bitboard(move.from()) ^ Bitboard(Square::G8);

                if constexpr (update_hash) {
                    m_hash ^= zobrist::piece_key(PieceType::Rook, Colour::Black, move.to());
                    m_hash ^= zobrist::piece_key(PieceType::King, Colour::Black, move.from());
                    m_hash ^= zobrist::piece_key(PieceType::Rook, Colour::Black, Square::F8);
                    m_hash ^= zobrist::piece_key(PieceType::King, Colour::Black, Square::G8);
                }
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

                if constexpr (update_hash) {
                    m_hash ^= zobrist::piece_key(PieceType::Rook, Colour::White, move.to());
                    m_hash ^= zobrist::piece_key(PieceType::King, Colour::White, move.from());
                    m_hash ^= zobrist::piece_key(PieceType::Rook, Colour::White, Square::D1);
                    m_hash ^= zobrist::piece_key(PieceType::King, Colour::White, Square::C1);
                }
            } else {
                // Rook
                m_colour[index(Colour::Black)] ^= Bitboard(move.to()) ^ Bitboard(Square::D8);
                m_piece[index(PieceType::Rook)] ^= Bitboard(move.to()) ^ Bitboard(Square::D8);
                // King
                m_colour[index(Colour::Black)] ^= Bitboard(move.from()) ^ Bitboard(Square::C8);
                m_piece[index(PieceType::King)] ^= Bitboard(move.from()) ^ Bitboard(Square::C8);

                if constexpr (update_hash) {
                    m_hash ^= zobrist::piece_key(PieceType::Rook, Colour::Black, move.to());
                    m_hash ^= zobrist::piece_key(PieceType::King, Colour::Black, move.from());
                    m_hash ^= zobrist::piece_key(PieceType::Rook, Colour::Black, Square::D8);
                    m_hash ^= zobrist::piece_key(PieceType::King, Colour::Black, Square::C8);
                }
            }
            break;
        case MoveType::EnPassant:
            m_halfmoves = 0;

            // Remove their pawn
            if (us == Colour::White) {
                const auto sq = static_cast<Square>(index(move.to()) - 8);
                m_colour[index(Colour::Black)] ^= Bitboard(sq);
                m_piece[index(PieceType::Pawn)] ^= Bitboard(sq);
                if constexpr (update_hash) {
                    m_hash ^= zobrist::piece_key(PieceType::Pawn, Colour::Black, sq);
                }
            } else {
                const auto sq = static_cast<Square>(index(move.to()) + 8);
                m_colour[index(Colour::White)] ^= Bitboard(sq);
                m_piece[index(PieceType::Pawn)] ^= Bitboard(sq);
                if constexpr (update_hash) {
                    m_hash ^= zobrist::piece_key(PieceType::Pawn, Colour::White, sq);
                }
            }

            // Move our pawn
            m_colour[index(us)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            m_piece[index(PieceType::Pawn)] ^= Bitboard(move.from()) ^ Bitboard(move.to());

            if constexpr (update_hash) {
                m_hash ^= zobrist::piece_key(PieceType::Pawn, us, move.to());
                m_hash ^= zobrist::piece_key(PieceType::Pawn, us, move.from());
            }
            break;
        case MoveType::Promo:
            m_halfmoves = 0;

            m_colour[index(us)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            m_piece[index(PieceType::Pawn)] ^= Bitboard(move.from());
            m_piece[index(move.promo())] ^= Bitboard(move.to());

            if constexpr (update_hash) {
                m_hash ^= zobrist::piece_key(move.promo(), us, move.to());
                m_hash ^= zobrist::piece_key(PieceType::Pawn, us, move.from());
            }
            break;
        case MoveType::PromoCapture:
            m_halfmoves = 0;

            m_colour[index(us)] ^= Bitboard(move.from()) ^ Bitboard(move.to());
            m_colour[index(them)] ^= Bitboard(move.to());
            m_piece[index(PieceType::Pawn)] ^= Bitboard(move.from());
            m_piece[index(move.promo())] ^= Bitboard(move.to());
            m_piece[index(move.captured())] ^= Bitboard(move.to());

            if constexpr (update_hash) {
                m_hash ^= zobrist::piece_key(move.captured(), them, move.to());
                m_hash ^= zobrist::piece_key(move.promo(), us, move.to());
                m_hash ^= zobrist::piece_key(PieceType::Pawn, us, move.from());
            }
            break;
    }

    if (move.to() == m_castle_rooks[0] || move.from() == m_castle_rooks[0] ||
        (us == Colour::White && piece == PieceType::King)) {
        m_castling &= ~(1 << static_cast<int>(CastleType::WhiteKingSide));
    }

    if (move.to() == m_castle_rooks[1] || move.from() == m_castle_rooks[1] ||
        (us == Colour::White && piece == PieceType::King)) {
        m_castling &= ~(1 << static_cast<int>(CastleType::WhiteQueenSide));
    }

    if (move.to() == m_castle_rooks[2] || move.from() == m_castle_rooks[2] ||
        (us == Colour::Black && piece == PieceType::King)) {
        m_castling &= ~(1 << static_cast<int>(CastleType::BlackKingSide));
    }

    if (move.to() == m_castle_rooks[3] || move.from() == m_castle_rooks[3] ||
        (us == Colour::Black && piece == PieceType::King)) {
        m_castling &= ~(1 << static_cast<int>(CastleType::BlackQueenSide));
    }

    if constexpr (update_hash) {
        m_hash ^= zobrist::castling_key(m_castling);
    }

    m_fullmoves += (m_turn == Colour::Black);
    m_turn = !m_turn;
}

template auto Position::makemove<true>(const Move &move) noexcept -> void;
template auto Position::makemove<false>(const Move &move) noexcept -> void;

}  // namespace chess

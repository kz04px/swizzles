#include "bitboard.hpp"
#include "magic.hpp"
#include "position.hpp"

namespace chess {

[[nodiscard]] auto Position::captures() const noexcept -> MoveList {
    MoveList movelist;

    const auto us = m_turn;
    const auto them = !us;
    const auto ksq = get_king(us);

    // Pawns
    if (us == Colour::White) {
        const auto pawns = get_pawns(Colour::White);
        const auto promo = pawns & Bitboard(Bitmask::Rank7);
        const auto nonpromo = pawns & Bitboard(~Bitmask::Rank7);

        // Captures - NW
        for (const auto to : nonpromo.north().west() & colour(Colour::Black)) {
            const auto fr = static_cast<Square>(index(to) - 7);
            movelist.emplace_back(MoveType::Capture, PieceType::Pawn, fr, to, piece_on(to));
        }

        // Captures - NE
        for (const auto to : nonpromo.north().east() & colour(Colour::Black)) {
            const auto fr = static_cast<Square>(index(to) - 9);
            movelist.emplace_back(MoveType::Capture, PieceType::Pawn, fr, to, piece_on(to));
        }

        // Captures promo - NW
        for (const auto to : promo.north().west() & colour(Colour::Black)) {
            const auto fr = static_cast<Square>(index(to) - 7);
            const auto captured = piece_on(to);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Queen);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Rook);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Bishop);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Knight);
        }

        // Captures promo - NE
        for (const auto to : promo.north().east() & colour(Colour::Black)) {
            const auto fr = static_cast<Square>(index(to) - 9);
            const auto captured = piece_on(to);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Queen);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Rook);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Bishop);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Knight);
        }
    } else {
        const auto pawns = get_pawns(Colour::Black);
        const auto promo = pawns & Bitboard(Bitmask::Rank2);
        const auto nonpromo = pawns & Bitboard(~Bitmask::Rank2);

        // Captures - SW
        for (const auto to : nonpromo.south().west() & colour(Colour::White)) {
            const auto fr = static_cast<Square>(index(to) + 9);
            movelist.emplace_back(MoveType::Capture, PieceType::Pawn, fr, to, piece_on(to));
        }

        // Captures - SE
        for (const auto to : nonpromo.south().east() & colour(Colour::White)) {
            const auto fr = static_cast<Square>(index(to) + 7);
            movelist.emplace_back(MoveType::Capture, PieceType::Pawn, fr, to, piece_on(to));
        }

        // Captures promo - SW
        for (const auto to : promo.south().west() & colour(Colour::White)) {
            const auto fr = static_cast<Square>(index(to) + 9);
            const auto captured = piece_on(to);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Queen);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Rook);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Bishop);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Knight);
        }

        // Captures promo - SE
        for (const auto to : promo.south().east() & colour(Colour::White)) {
            const auto fr = static_cast<Square>(index(to) + 7);
            const auto captured = piece_on(to);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Queen);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Rook);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Bishop);
            movelist.emplace_back(MoveType::PromoCapture, PieceType::Pawn, fr, to, captured, PieceType::Knight);
        }
    }

    // Knights
    for (const auto knights = get_knights(us); const auto fr : knights) {
        const auto moves = Bitboard(fr).knight();

        for (const auto mask = moves & colour(them); const auto to : mask) {
            movelist.emplace_back(MoveType::Capture, PieceType::Knight, fr, to, piece_on(to));
        }
    }

    // Bishops
    for (const auto bishops = get_bishops(us); const auto fr : bishops) {
        const auto moves = magic::bishop_moves(fr, get_occupied());

        for (const auto mask = moves & colour(them); const auto to : mask) {
            movelist.emplace_back(MoveType::Capture, PieceType::Bishop, fr, to, piece_on(to));
        }
    }

    // Rooks
    for (const auto rooks = get_rooks(us); const auto fr : rooks) {
        const auto moves = magic::rook_moves(fr, get_occupied());

        for (const auto mask = moves & colour(them); const auto to : mask) {
            movelist.emplace_back(MoveType::Capture, PieceType::Rook, fr, to, piece_on(to));
        }
    }

    // Queens
    for (const auto queens = get_queens(us); const auto fr : queens) {
        const auto moves = magic::queen_moves(fr, get_occupied());

        for (const auto mask = moves & colour(them); const auto to : mask) {
            movelist.emplace_back(MoveType::Capture, PieceType::Queen, fr, to, piece_on(to));
        }
    }

    // King
    for (const auto to : Bitboard(ksq).adjacent() & colour(them)) {
        movelist.emplace_back(MoveType::Capture, PieceType::King, ksq, to, piece_on(to));
    }

    // En passant
    if (m_enpassant != Square::None) {
        if (us == Colour::White) {
            // NW
            if (get_pawns(Colour::White).north().west() & Bitboard(m_enpassant)) {
                const auto fr = static_cast<Square>(index(m_enpassant) - 7);
                movelist.emplace_back(MoveType::EnPassant, PieceType::Pawn, fr, m_enpassant, PieceType::Pawn);
            }
            // NE
            if (get_pawns(Colour::White).north().east() & Bitboard(m_enpassant)) {
                const auto fr = static_cast<Square>(index(m_enpassant) - 9);
                movelist.emplace_back(MoveType::EnPassant, PieceType::Pawn, fr, m_enpassant, PieceType::Pawn);
            }
        } else {
            // SW
            if (get_pawns(Colour::Black).south().west() & Bitboard(m_enpassant)) {
                const auto fr = static_cast<Square>(index(m_enpassant) + 9);
                movelist.emplace_back(MoveType::EnPassant, PieceType::Pawn, fr, m_enpassant, PieceType::Pawn);
            }
            // SE
            if (get_pawns(Colour::Black).south().east() & Bitboard(m_enpassant)) {
                const auto fr = static_cast<Square>(index(m_enpassant) + 7);
                movelist.emplace_back(MoveType::EnPassant, PieceType::Pawn, fr, m_enpassant, PieceType::Pawn);
            }
        }
    }

    return movelist;
}

}  // namespace chess

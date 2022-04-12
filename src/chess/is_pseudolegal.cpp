#include "bitboard.hpp"
#include "magic.hpp"
#include "position.hpp"

namespace chess {

[[nodiscard]] constexpr auto path_between(Square a, Square b) noexcept -> Bitboard {
    if (b < a) {
        std::swap(a, b);
    }
    return Bitboard((Bitmask::Full >> (63 - index(a))) ^ (Bitmask::Full >> (64 - index(b))));
}

[[nodiscard]] auto Position::is_pseudolegal(const Move &move) const noexcept -> bool {
    const auto us = turn();
    const auto them = !us;
    const auto ksq = get_king(us);
    const auto in_check = is_attacked(ksq, them);

    if (!(colour(us) & Bitboard(move.from()))) {
        return false;
    }

    if (us == Colour::White) {
        // White king side castle
        if (move.type() == MoveType::KSC) {
            if (in_check) {
                return false;
            }

            if (!can_castle(CastleType::WhiteKingSide)) {
                return false;
            }

            if (!(Bitboard(move.to()) & get_rooks(us))) {
                return false;
            }

            const auto blockers = get_occupied() ^ Bitboard(ksq) ^ Bitboard(m_castle_rooks[0]);

            const auto king_path = (path_between(ksq, Square::G1) | Bitboard(Square::G1)) & ~Bitboard(ksq);
            const auto king_path_clear = (king_path & blockers).empty();

            const auto rook_path = path_between(Square::F1, m_castle_rooks[0]) | Bitboard(Square::F1);
            const auto rook_path_clear = (rook_path & blockers).empty();

            if (!king_path_clear) {
                return false;
            }

            if (!rook_path_clear) {
                return false;
            }

            if (is_attacked(king_path, them)) {
                return false;
            }

            return true;
        }

        // White queen side castle
        if (move.type() == MoveType::QSC) {
            if (in_check) {
                return false;
            }

            if (!can_castle(CastleType::WhiteQueenSide)) {
                return false;
            }

            if (!(Bitboard(move.to()) & get_rooks(us))) {
                return false;
            }

            const auto blockers = get_occupied() ^ Bitboard(ksq) ^ Bitboard(m_castle_rooks[1]);

            const auto king_path = (path_between(ksq, Square::C1) | Bitboard(Square::C1)) & ~Bitboard(ksq);
            const auto king_path_clear = (king_path & blockers).empty();

            const auto rook_path = path_between(Square::D1, m_castle_rooks[1]) | Bitboard(Square::D1);
            const auto rook_path_clear = (rook_path & blockers).empty();

            if (!king_path_clear) {
                return false;
            }

            if (!rook_path_clear) {
                return false;
            }

            if (is_attacked(king_path, them)) {
                return false;
            }

            return true;
        }
    } else {
        // Black king side castle
        if (move.type() == MoveType::KSC) {
            if (in_check) {
                return false;
            }

            if (!can_castle(CastleType::BlackKingSide)) {
                return false;
            }

            if (!(Bitboard(move.to()) & get_rooks(us))) {
                return false;
            }

            const auto blockers = get_occupied() ^ Bitboard(ksq) ^ Bitboard(m_castle_rooks[2]);

            const auto king_path = (path_between(ksq, Square::G8) | Bitboard(Square::G8)) & ~Bitboard(ksq);
            const auto king_path_clear = (king_path & blockers).empty();

            const auto rook_path = path_between(Square::F8, m_castle_rooks[2]) | Bitboard(Square::F8);
            const auto rook_path_clear = (rook_path & blockers).empty();

            if (!king_path_clear) {
                return false;
            }

            if (!rook_path_clear) {
                return false;
            }

            if (is_attacked(king_path, them)) {
                return false;
            }

            return true;
        }

        // Black queen side castle
        if (move.type() == MoveType::QSC) {
            if (in_check) {
                return false;
            }

            if (!can_castle(CastleType::BlackQueenSide)) {
                return false;
            }

            if (!(Bitboard(move.to()) & get_rooks(us))) {
                return false;
            }

            const auto blockers = get_occupied() ^ Bitboard(ksq) ^ Bitboard(m_castle_rooks[3]);

            const auto king_path = (path_between(ksq, Square::C8) | Bitboard(Square::C8)) & ~Bitboard(ksq);
            const auto king_path_clear = (king_path & blockers).empty();

            const auto rook_path = path_between(Square::D8, m_castle_rooks[3]) | Bitboard(Square::D8);
            const auto rook_path_clear = (rook_path & blockers).empty();

            if (!king_path_clear) {
                return false;
            }

            if (!rook_path_clear) {
                return false;
            }

            if (is_attacked(king_path, them)) {
                return false;
            }

            return true;
        }
    }

    // Can't capture our own pieces
    if (colour(us) & Bitboard(move.to())) {
        return false;
    }

    // Piece type has to match
    if (piece_on(move.from()) != move.piece()) {
        return false;
    }

    // Captured piece must be there
    if (move.type() != MoveType::EnPassant && piece_on(move.to()) != move.captured()) {
        return false;
    }

    const auto blockers = get_occupied();
    switch (move.piece()) {
        case PieceType::Pawn: {
            if (us == Colour::White && index(move.to()) < index(move.from())) {
                return false;
            }
            if (us == Colour::Black && index(move.to()) > index(move.from())) {
                return false;
            }

            if (move.type() == MoveType::Quiet) {
                if (blockers & Bitboard(move.to())) {
                    return false;
                }
            } else if (move.type() == MoveType::Double) {
                const auto mask = turn() == Colour::White ? Bitboard(move.to()) | Bitboard(move.to()).south()
                                                          : Bitboard(move.to()) | Bitboard(move.to()).north();
                if (blockers & mask) {
                    return false;
                }
            } else if (move.type() == MoveType::EnPassant) {
                if (enpassant() != move.to()) {
                    return false;
                }
            }
            break;
        }
        case PieceType::Knight: {
            const auto moves = Bitboard(move.from()).knight() & (~get_us());
            if (!(moves & Bitboard(move.to()))) {
                return false;
            }
            break;
        }
        case PieceType::Bishop: {
            const auto moves = magic::bishop_moves(move.from(), blockers) & (~get_us());
            if (!(moves & Bitboard(move.to()))) {
                return false;
            }
            break;
        }
        case PieceType::Rook: {
            const auto moves = magic::rook_moves(move.from(), blockers) & (~get_us());
            if (!(moves & Bitboard(move.to()))) {
                return false;
            }
            break;
        }
        case PieceType::Queen: {
            const auto moves = magic::queen_moves(move.from(), blockers) & (~get_us());
            if (!(moves & Bitboard(move.to()))) {
                return false;
            }
            break;
        }
        case PieceType::King: {
            const auto moves = Bitboard(move.from()).adjacent() & (~get_us());
            if (!(moves & Bitboard(move.to()))) {
                return false;
            }
            break;
        }
        case PieceType::None:
            return false;
        default:
            return false;
    }

    return true;
}

}  // namespace chess

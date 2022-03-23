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

static_assert(path_between(Square::A1, Square::G1) == Bitboard(0x3EULL));
static_assert(path_between(Square::A1, Square::H1) == Bitboard(0x7EULL));
static_assert(path_between(Square::B1, Square::H1) == Bitboard(0x7CULL));
static_assert(path_between(Square::C1, Square::D1) == Bitboard(0x0ULL));
static_assert(path_between(Square::D1, Square::C1) == Bitboard(0x0ULL));
static_assert(path_between(Square::D1, Square::F1) == Bitboard(0x10ULL));
static_assert(path_between(Square::F1, Square::D1) == Bitboard(0x10ULL));
static_assert(path_between(Square::G1, Square::A1) == Bitboard(0x3EULL));
static_assert(path_between(Square::H1, Square::A1) == Bitboard(0x7EULL));
static_assert(path_between(Square::H8, Square::D8) == Bitboard(0x7000000000000000ULL));
static_assert(path_between(Square::D8, Square::H8) == Bitboard(0x7000000000000000ULL));

/*
[[nodiscard]] constexpr auto path_between2(const Square a, const Square b) noexcept -> Bitboard {
    const auto left = ~chess::Bitboard(0xffffffffffffffffULL << chess::index(a));
    const auto right = chess::Bitboard(0xfffffffffffffffeULL << chess::index(b));
    const auto combo = ~(right ^ left) | chess::Bitboard(a) | chess::Bitboard(b);
    return combo;
}

static_assert(path_between2(Square::C1, Square::F1) == Bitboard(0x3CULL));
static_assert(path_between2(Square::F1, Square::C1) == Bitboard(0x3CULL));
static_assert(path_between2(Square::C8, Square::F8) == Bitboard(0x3C00000000000000ULL));
static_assert(path_between2(Square::F8, Square::C8) == Bitboard(0x3C00000000000000ULL));
*/

[[nodiscard]] auto Position::movegen() const noexcept -> MoveList {
    MoveList movelist;

    const auto us = m_turn;
    const auto them = !us;
    const auto ksq = get_king(us);
    const auto in_check = is_attacked(ksq, them);

    // Pawns
    if (us == Colour::White) {
        const auto pawns = get_pawns(Colour::White);
        const auto promo = pawns & Bitboard(Bitmask::Rank7);
        const auto nonpromo = pawns & Bitboard(~Bitmask::Rank7);
        const auto doubles = (pawns.north() & get_empty()).north() & Bitboard(Bitmask::Rank4) & get_empty();

        // Singles - Nonpromo
        for (const auto to : nonpromo.north() & get_empty()) {
            const auto fr = static_cast<Square>(index(to) - 8);
            movelist.emplace_back(MoveType::Quiet, PieceType::Pawn, fr, to);
        }

        // Singles - Promo
        for (const auto to : promo.north() & get_empty()) {
            const auto fr = static_cast<Square>(index(to) - 8);
            movelist.emplace_back(MoveType::Promo, PieceType::Pawn, fr, to, PieceType::None, PieceType::Queen);
            movelist.emplace_back(MoveType::Promo, PieceType::Pawn, fr, to, PieceType::None, PieceType::Rook);
            movelist.emplace_back(MoveType::Promo, PieceType::Pawn, fr, to, PieceType::None, PieceType::Bishop);
            movelist.emplace_back(MoveType::Promo, PieceType::Pawn, fr, to, PieceType::None, PieceType::Knight);
        }

        // Double
        for (const auto to : doubles) {
            const auto fr = static_cast<Square>(index(to) - 16);
            movelist.emplace_back(MoveType::Double, PieceType::Pawn, fr, to);
        }

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
        const auto doubles = (pawns.south() & get_empty()).south() & Bitboard(Bitmask::Rank5) & get_empty();

        // Singles - Nonpromo
        for (const auto to : nonpromo.south() & get_empty()) {
            const auto fr = static_cast<Square>(index(to) + 8);
            movelist.emplace_back(MoveType::Quiet, PieceType::Pawn, fr, to);
        }

        // Singles - Promo
        for (const auto to : promo.south() & get_empty()) {
            const auto fr = static_cast<Square>(index(to) + 8);
            movelist.emplace_back(MoveType::Promo, PieceType::Pawn, fr, to, PieceType::None, PieceType::Queen);
            movelist.emplace_back(MoveType::Promo, PieceType::Pawn, fr, to, PieceType::None, PieceType::Rook);
            movelist.emplace_back(MoveType::Promo, PieceType::Pawn, fr, to, PieceType::None, PieceType::Bishop);
            movelist.emplace_back(MoveType::Promo, PieceType::Pawn, fr, to, PieceType::None, PieceType::Knight);
        }

        // Double
        for (const auto to : doubles) {
            const auto fr = static_cast<Square>(index(to) + 16);
            movelist.emplace_back(MoveType::Double, PieceType::Pawn, fr, to);
        }

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

        // Capture
        for (const auto mask = moves & colour(them); const auto to : mask) {
            movelist.emplace_back(MoveType::Capture, PieceType::Knight, fr, to, piece_on(to));
        }

        // Quiet
        for (const auto mask = moves & get_empty(); const auto to : mask) {
            movelist.emplace_back(MoveType::Quiet, PieceType::Knight, fr, to);
        }
    }

    // Bishops
    for (const auto bishops = get_bishops(us); const auto fr : bishops) {
        const auto moves = magic::bishop_moves(fr, get_occupied());

        // Capture
        for (const auto mask = moves & colour(them); const auto to : mask) {
            movelist.emplace_back(MoveType::Capture, PieceType::Bishop, fr, to, piece_on(to));
        }

        // Quiet
        for (const auto mask = moves & get_empty(); const auto to : mask) {
            movelist.emplace_back(MoveType::Quiet, PieceType::Bishop, fr, to);
        }
    }

    // Rooks
    for (const auto rooks = get_rooks(us); const auto fr : rooks) {
        const auto moves = magic::rook_moves(fr, get_occupied());

        // Capture
        for (const auto mask = moves & colour(them); const auto to : mask) {
            movelist.emplace_back(MoveType::Capture, PieceType::Rook, fr, to, piece_on(to));
        }

        // Quiet
        for (const auto mask = moves & get_empty(); const auto to : mask) {
            movelist.emplace_back(MoveType::Quiet, PieceType::Rook, fr, to);
        }
    }

    // Queens
    for (const auto queens = get_queens(us); const auto fr : queens) {
        const auto moves = magic::queen_moves(fr, get_occupied());

        // Capture
        for (const auto mask = moves & colour(them); const auto to : mask) {
            movelist.emplace_back(MoveType::Capture, PieceType::Queen, fr, to, piece_on(to));
        }

        // Quiet
        for (const auto mask = moves & get_empty(); const auto to : mask) {
            movelist.emplace_back(MoveType::Quiet, PieceType::Queen, fr, to);
        }
    }

    // King
    for (const auto to : Bitboard(ksq).adjacent() & colour(them)) {
        movelist.emplace_back(MoveType::Capture, PieceType::King, ksq, to, piece_on(to));
    }
    for (const auto to : Bitboard(ksq).adjacent() & get_empty()) {
        movelist.emplace_back(MoveType::Quiet, PieceType::King, ksq, to);
    }

    // Castling
    if (us == Colour::White) {
        if (!in_check && can_castle(CastleType::WhiteKingSide)) {
            const auto blockers = get_occupied() ^ Bitboard(ksq) ^ Bitboard(m_castle_rooks[0]);

            const auto king_path = (path_between(ksq, Square::G1) | Bitboard(Square::G1)) & ~Bitboard(ksq);
            const auto king_path_clear = (king_path & blockers).empty();

            const auto rook_path = path_between(Square::F1, m_castle_rooks[0]) | Bitboard(Square::F1);
            const auto rook_path_clear = (rook_path & blockers).empty();

            if (king_path_clear && rook_path_clear && !is_attacked(king_path, them)) {
                movelist.emplace_back(MoveType::KSC, PieceType::King, ksq, m_castle_rooks[0]);
            }
        }
        if (!in_check && can_castle(CastleType::WhiteQueenSide)) {
            const auto blockers = get_occupied() ^ Bitboard(ksq) ^ Bitboard(m_castle_rooks[1]);

            const auto king_path = (path_between(ksq, Square::C1) | Bitboard(Square::C1)) & ~Bitboard(ksq);
            const auto king_path_clear = (king_path & blockers).empty();

            const auto rook_path = path_between(Square::D1, m_castle_rooks[1]) | Bitboard(Square::D1);
            const auto rook_path_clear = (rook_path & blockers).empty();

            if (king_path_clear && rook_path_clear && !is_attacked(king_path, them)) {
                movelist.emplace_back(MoveType::QSC, PieceType::King, ksq, m_castle_rooks[1]);
            }
        }
    } else {
        if (!in_check && can_castle(CastleType::BlackKingSide)) {
            const auto blockers = get_occupied() ^ Bitboard(ksq) ^ Bitboard(m_castle_rooks[2]);

            const auto king_path = (path_between(ksq, Square::G8) | Bitboard(Square::G8)) & ~Bitboard(ksq);
            const auto king_path_clear = (king_path & blockers).empty();

            const auto rook_path = path_between(Square::F8, m_castle_rooks[2]) | Bitboard(Square::F8);
            const auto rook_path_clear = (rook_path & blockers).empty();

            if (king_path_clear && rook_path_clear && !is_attacked(king_path, them)) {
                movelist.emplace_back(MoveType::KSC, PieceType::King, ksq, m_castle_rooks[2]);
            }
        }
        if (!in_check && can_castle(CastleType::BlackQueenSide)) {
            const auto blockers = get_occupied() ^ Bitboard(ksq) ^ Bitboard(m_castle_rooks[3]);

            const auto king_path = (path_between(ksq, Square::C8) | Bitboard(Square::C8)) & ~Bitboard(ksq);
            const auto king_path_clear = (king_path & blockers).empty();

            const auto rook_path = path_between(Square::D8, m_castle_rooks[3]) | Bitboard(Square::D8);
            const auto rook_path_clear = (rook_path & blockers).empty();

            if (king_path_clear && rook_path_clear && !is_attacked(king_path, them)) {
                movelist.emplace_back(MoveType::QSC, PieceType::King, ksq, m_castle_rooks[3]);
            }
        }
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

#ifndef CHESS_VALIDATE_HPP
#define CHESS_VALIDATE_HPP

#include <stdexcept>
#include "colour.hpp"
#include "move.hpp"
#include "position.hpp"

namespace chess::validate {

constexpr auto move(const Move &move) -> void {
    if (move.captured() == PieceType::King) {
        throw std::logic_error("Move can't capture kings");
    }

    if (move.to() == Square::None) {
        throw std::logic_error("Move can't go to the none square");
    }

    if (move.from() == Square::None) {
        throw std::logic_error("Move can't come from the none square");
    }

    if (move.to() == move.from()) {
        throw std::logic_error("Move to square can't equal from square");
    }

    if (move.type() == MoveType::KSC || move.type() == MoveType::QSC) {
        if (move.piece() != PieceType::King) {
            throw std::logic_error("Only the king can castle");
        }

        if (!(Bitboard(move.to()) & Bitboard(Bitmask::Rank1 | Bitmask::Rank8))) {
            throw std::logic_error("Castling must be at the end of the board");
        }

        if (!(Bitboard(move.from()) & Bitboard(Bitmask::FileA | Bitmask::FileH))) {
            throw std::logic_error("Castling can't happem from the sides of the board");
        }
    }

    if (move.type() == MoveType::Promo) {
        if (move.piece() != PieceType::Pawn) {
            throw std::logic_error("Promoting pieces must be pawns");
        }

        if (move.captured() != PieceType::Pawn) {
            throw std::logic_error("Quiet promotion can't capture");
        }

        if ((move.promo() != PieceType::Knight) && (move.promo() != PieceType::Bishop) &&
            (move.promo() != PieceType::Rook) && (move.promo() != PieceType::Queen)) {
            throw std::logic_error("Promotion must be to NBRQ");
        }

        if (!(Bitboard(move.to()) & Bitboard(Bitmask::Rank1 | Bitmask::Rank8))) {
            throw std::logic_error("Promotion must be at the end of the board");
        }
    }

    if (move.type() == MoveType::PromoCapture) {
        if (move.piece() != PieceType::Pawn) {
            throw std::logic_error("Promoting pieces must be pawns");
        }

        if (move.captured() == PieceType::None) {
            throw std::logic_error("Capturing promotion must capture");
        }

        if ((move.promo() != PieceType::Knight) && (move.promo() != PieceType::Bishop) &&
            (move.promo() != PieceType::Rook) && (move.promo() != PieceType::Queen)) {
            throw std::logic_error("Promotion must be to NBRQ");
        }

        if (!(Bitboard(move.to()) & Bitboard(Bitmask::Rank1 | Bitmask::Rank8))) {
            throw std::logic_error("Promotion must be at the end of the board");
        }
    }
}

const auto mask_east = [](const Square sq) noexcept -> Bitboard {
    return Bitboard::rank_of(sq) & Bitboard(0xFEULL << index(sq));
};

const auto mask_west = [](const Square sq) noexcept -> Bitboard {
    return Bitboard::rank_of(sq) & (Bitboard::rank_of(sq) >> (8 - file(sq)));
};

static_assert(mask_east(Square::A1) == Bitboard(0xFEULL));
static_assert(mask_west(Square::A1) == Bitboard());
static_assert(mask_east(Square::E1) == Bitboard(0xE0ULL));
static_assert(mask_west(Square::E1) == Bitboard(0xFULL));
static_assert(mask_east(Square::E5) == Bitboard(0xE000000000ULL));
static_assert(mask_west(Square::E5) == Bitboard(0xF00000000ULL));

constexpr auto position(const Position &pos) -> void {
    const auto wksq = pos.get_king(Colour::White);
    const auto bksq = pos.get_king(Colour::Black);

    // Counts
    if (pos.get_white().count() > 16) {
        throw std::logic_error("Too many white pieces");
    }

    if (pos.get_black().count() > 16) {
        throw std::logic_error("Too many black pieces");
    }

    if (pos.get_pawns(Colour::White).count() > 8) {
        throw std::logic_error("Too many white pawns");
    }

    if (pos.get_pawns(Colour::Black).count() > 8) {
        throw std::logic_error("Too many black pawns");
    }

    if (pos.get_kings(Colour::White).count() != 1) {
        throw std::logic_error("Should be 1 white king");
    }

    if (pos.get_kings(Colour::Black).count() != 1) {
        throw std::logic_error("Should be 1 black king");
    }

    // Overlaps
    if (pos.get_white() & pos.get_black()) {
        throw std::logic_error("White and black piece overlap");
    }

    if (pos.get_pawns() & pos.get_knights()) {
        throw std::logic_error("Pawn and knight piece overlap");
    }

    if (pos.get_pawns() & pos.get_bishops()) {
        throw std::logic_error("Pawn and bishop piece overlap");
    }

    if (pos.get_pawns() & pos.get_rooks()) {
        throw std::logic_error("Pawn and rook piece overlap");
    }

    if (pos.get_pawns() & pos.get_queens()) {
        throw std::logic_error("Pawn and queen piece overlap");
    }

    if (pos.get_pawns() & pos.get_kings()) {
        throw std::logic_error("Pawn and kings piece overlap");
    }

    if (pos.get_knights() & pos.get_bishops()) {
        throw std::logic_error("Knight and bishop piece overlap");
    }

    if (pos.get_knights() & pos.get_rooks()) {
        throw std::logic_error("Knight and rook piece overlap");
    }

    if (pos.get_knights() & pos.get_queens()) {
        throw std::logic_error("Knight and queen piece overlap");
    }

    if (pos.get_knights() & pos.get_kings()) {
        throw std::logic_error("Knight and kings piece overlap");
    }

    if (pos.get_bishops() & pos.get_rooks()) {
        throw std::logic_error("Bishop and rook piece overlap");
    }

    if (pos.get_bishops() & pos.get_queens()) {
        throw std::logic_error("Bishop and queen piece overlap");
    }

    if (pos.get_bishops() & pos.get_kings()) {
        throw std::logic_error("Bishop and kings piece overlap");
    }

    if (pos.get_rooks() & pos.get_queens()) {
        throw std::logic_error("Rooks and queen piece overlap");
    }

    if (pos.get_rooks() & pos.get_kings()) {
        throw std::logic_error("Rooks and kings piece overlap");
    }

    if (pos.get_queens() & pos.get_kings()) {
        throw std::logic_error("Queens and kings piece overlap");
    }

    // Castling
    if (rank(pos.castle_rook(CastleType::WhiteKingSide)) != 0) {
        throw std::logic_error("White Kingside rook square for castling must be on the 1st rank");
    }

    if (rank(pos.castle_rook(CastleType::WhiteQueenSide)) != 0) {
        throw std::logic_error("White Queenside rook square for castling must be on the 1st rank");
    }

    if (file(pos.castle_rook(CastleType::WhiteQueenSide)) + 1 >= file(pos.castle_rook(CastleType::WhiteKingSide))) {
        throw std::logic_error("White king side castling rook must be further right than the queen side castle rook");
    }

    if (const auto ep = pos.enpassant(); ep != Square::None && pos.turn() == Colour::White && rank(ep) != 5) {
        throw std::logic_error("En passant square must be on the 6th rank on white's turn");
    }

    if (const auto ep = pos.enpassant(); ep != Square::None && pos.turn() == Colour::Black && rank(ep) != 2) {
        throw std::logic_error("En passant square must be on the 3rd rank on black's turn");
    }

    if (pos.can_castle(CastleType::WhiteKingSide)) {
        // King must be on the 1st rank
        if (!(Bitboard(wksq) & Bitboard(Bitmask::Rank1))) {
            throw std::logic_error("King must be on the 1st rank to castle");
        }

        // There must be a friendly rook to the east
        if (!(pos.get_rooks(Colour::White) & mask_east(wksq))) {
            throw std::logic_error("There must be a rook to our east if we can KSC");
        }
    }

    if (pos.can_castle(CastleType::WhiteQueenSide)) {
        // King must be on the 1st rank
        if (!(Bitboard(wksq) & Bitboard(Bitmask::Rank1))) {
            throw std::logic_error("King must be on the 1st rank to castle");
        }

        // There must be a friendly rook to the west
        if (!(pos.get_rooks(Colour::White) & mask_west(wksq))) {
            throw std::logic_error("There must be a rook to our west if we can QSC");
        }
    }

    if (pos.can_castle(CastleType::BlackKingSide)) {
        // King must be on the 8th rank
        if (!(Bitboard(bksq) & Bitboard(Bitmask::Rank8))) {
            throw std::logic_error("King must be on the 8th rank to castle");
        }

        // There must be a friendly rook to the east
        if (!(pos.get_rooks(Colour::Black) & mask_east(bksq))) {
            throw std::logic_error("There must be a rook to our east if we can KSC");
        }
    }

    if (pos.can_castle(CastleType::BlackQueenSide)) {
        // King must be on the 8th rank
        if (!(Bitboard(bksq) & Bitboard(Bitmask::Rank8))) {
            throw std::logic_error("King must be on the 8th rank to castle");
        }

        // There must be a friendly rook to the west
        if (!(pos.get_rooks(Colour::Black) & mask_west(bksq))) {
            throw std::logic_error("There must be a rook to our west if we can QSC");
        }
    }
}

}  // namespace chess::validate

#endif

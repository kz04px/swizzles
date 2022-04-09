#ifndef CHESS_POSITION_HPP
#define CHESS_POSITION_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>
#include "bitboard.hpp"
#include "colour.hpp"
#include "move.hpp"
#include "movelist.hpp"
#include "square.hpp"
#include "zobrist.hpp"

namespace chess {

enum class CastleType : int
{
    WhiteKingSide = 0,
    WhiteQueenSide,
    BlackKingSide,
    BlackQueenSide,
};

class Position {
   public:
    [[nodiscard]] Position() noexcept = default;

    [[nodiscard]] explicit Position(const std::string_view fen) noexcept {
        set_fen(fen);
    }

    [[nodiscard]] constexpr auto turn() const noexcept -> Colour {
        return m_turn;
    }

    [[nodiscard]] constexpr auto halfmoves() const noexcept -> std::size_t {
        return m_halfmoves;
    }

    [[nodiscard]] constexpr auto fullmoves() const noexcept -> std::size_t {
        return m_fullmoves;
    }

    [[nodiscard]] constexpr auto can_castle(const CastleType type) const noexcept -> bool {
        return m_castling & (1 << static_cast<int>(type));
    }

    [[nodiscard]] constexpr auto enpassant() const noexcept -> Square {
        return m_enpassant;
    }

    [[nodiscard]] auto get_fen(const bool chess960 = false) const noexcept -> std::string;

    [[nodiscard]] auto movegen() const noexcept -> MoveList;

    [[nodiscard]] auto captures() const noexcept -> MoveList;

    [[nodiscard]] auto is_attacked(const Square sq, const Colour side) const noexcept -> bool;

    [[nodiscard]] auto is_attacked(const Bitboard bb, const Colour side) const noexcept -> bool;

    [[nodiscard]] constexpr auto hash() const noexcept -> zobrist::hash_type {
        return m_hash;
    }

    [[nodiscard]] constexpr auto castle_rook(const CastleType type) const noexcept -> Square {
        return m_castle_rooks[static_cast<int>(type)];
    }

    [[nodiscard]] auto predict_hash(const Move move) const noexcept -> zobrist::hash_type;

    [[nodiscard]] auto num_repeats() const noexcept -> int {
        int repeats = 1;
        for (std::size_t i = 2; i <= m_history.size() && i <= m_halfmoves; i += 2) {
            if (m_history[m_history.size() - i].hash == m_hash) {
                repeats++;
            }
        }
        return repeats;
    }

    [[nodiscard]] constexpr auto get_white() const noexcept -> Bitboard {
        return m_colour[0];
    }

    [[nodiscard]] constexpr auto get_black() const noexcept -> Bitboard {
        return m_colour[1];
    }

    [[nodiscard]] constexpr auto colour(const Colour side) const noexcept -> Bitboard {
        return m_colour[static_cast<std::size_t>(side)];
    }

    [[nodiscard]] constexpr auto get_empty() const noexcept -> Bitboard {
        return ~(m_colour[0] | m_colour[1]);
    }

    [[nodiscard]] constexpr auto get_occupied() const noexcept -> Bitboard {
        return m_colour[0] | m_colour[1];
    }

    [[nodiscard]] constexpr auto is_occupied(const Square sq) const noexcept -> bool {
        return (m_colour[0] | m_colour[1]).is_occupied(sq);
    }

    [[nodiscard]] constexpr auto is_empty(const Square sq) const noexcept -> bool {
        return (m_colour[0] | m_colour[1]).is_empty(sq);
    }

    [[nodiscard]] constexpr auto is_empty(const Bitboard bb) const noexcept -> bool {
        return ((m_colour[0] | m_colour[1]) & bb).empty();
    }

    [[nodiscard]] constexpr auto get_colours() const noexcept -> std::array<Bitboard, 2> {
        return m_colour;
    }

    [[nodiscard]] constexpr auto get_pieces() const noexcept -> std::array<Bitboard, 6> {
        return m_piece;
    }

    [[nodiscard]] constexpr auto get_pawns() const noexcept -> Bitboard {
        return m_piece[index(PieceType::Pawn)];
    }

    [[nodiscard]] constexpr auto get_knights() const noexcept -> Bitboard {
        return m_piece[index(PieceType::Knight)];
    }

    [[nodiscard]] constexpr auto get_bishops() const noexcept -> Bitboard {
        return m_piece[index(PieceType::Bishop)];
    }

    [[nodiscard]] constexpr auto get_rooks() const noexcept -> Bitboard {
        return m_piece[index(PieceType::Rook)];
    }

    [[nodiscard]] constexpr auto get_queens() const noexcept -> Bitboard {
        return m_piece[index(PieceType::Queen)];
    }

    [[nodiscard]] constexpr auto get_kings() const noexcept -> Bitboard {
        return m_piece[index(PieceType::King)];
    }

    [[nodiscard]] constexpr auto get_pawns(const Colour c) const noexcept -> Bitboard {
        return colour(c) & get_pawns();
    }

    [[nodiscard]] constexpr auto get_knights(const Colour c) const noexcept -> Bitboard {
        return colour(c) & get_knights();
    }

    [[nodiscard]] constexpr auto get_bishops(const Colour c) const noexcept -> Bitboard {
        return colour(c) & get_bishops();
    }

    [[nodiscard]] constexpr auto get_rooks(const Colour c) const noexcept -> Bitboard {
        return colour(c) & get_rooks();
    }

    [[nodiscard]] constexpr auto get_queens(const Colour c) const noexcept -> Bitboard {
        return colour(c) & get_queens();
    }

    [[nodiscard]] constexpr auto get_kings(const Colour c) const noexcept -> Bitboard {
        return colour(c) & get_kings();
    }

    [[nodiscard]] constexpr auto get_piece(const PieceType piece) const noexcept -> Bitboard {
        return m_piece[static_cast<std::size_t>(piece)];
    }

    [[nodiscard]] constexpr auto get_king(const Colour side) const noexcept -> Square {
        return (colour(side) & get_kings()).lsb();
    }

    [[nodiscard]] constexpr auto piece_on(const Square sq) const noexcept -> PieceType {
        const auto bb = Bitboard(sq);
        if (bb & get_pawns()) {
            return PieceType::Pawn;
        } else if (bb & get_knights()) {
            return PieceType::Knight;
        } else if (bb & get_bishops()) {
            return PieceType::Bishop;
        } else if (bb & get_rooks()) {
            return PieceType::Rook;
        } else if (bb & get_queens()) {
            return PieceType::Queen;
        } else if (bb & get_kings()) {
            return PieceType::King;
        } else {
            return PieceType::None;
        }
    }

    [[nodiscard]] auto calculate_hash() const noexcept -> zobrist::hash_type;

    template <bool update_hash = true>
    auto makemove(const Move &move) noexcept -> void;

    auto undomove() noexcept -> void;

    template <bool update_hash = true>
    auto makenull() noexcept -> void {
        push_history(Move());

        if constexpr (update_hash) {
            if (m_enpassant != Square::None) {
                m_hash ^= zobrist::ep_key(m_enpassant);
            }
            m_hash ^= zobrist::turn_key();
        }

        m_turn = !m_turn;
        m_enpassant = Square::None;
        m_halfmoves = 0;
    }

    auto undonull() noexcept -> void {
        m_turn = !m_turn;
        m_fullmoves -= (m_turn == Colour::Black);
        restore_history();
    }

    auto set_fen(const std::string_view fen) -> void;

   private:
    struct History {
        [[nodiscard]] constexpr History(const Position &pos, const Move &m)
            : move(m),
              castling(pos.m_castling),
              halfmoves(pos.m_halfmoves),
              enpassant(pos.m_enpassant),
              hash(pos.hash()) {
        }

        [[nodiscard]] constexpr History(const Move &m,
                                        const int c,
                                        const int hm,
                                        const Square ep,
                                        const zobrist::hash_type h)
            : move(m), castling(c), halfmoves(hm), enpassant(ep), hash(h) {
        }

        Move move;
        int castling;
        std::size_t halfmoves;
        Square enpassant;
        zobrist::hash_type hash;
    };

    auto push_history(const Move &move) noexcept -> void {
        m_history.emplace_back(*this, move);
    }

    auto restore_history() noexcept -> void {
        m_castling = m_history.back().castling;
        m_halfmoves = m_history.back().halfmoves;
        m_enpassant = m_history.back().enpassant;
        m_hash = m_history.back().hash;
        m_history.pop_back();
    }

    std::array<Bitboard, 2> m_colour = {};
    std::array<Bitboard, 6> m_piece = {};
    std::size_t m_halfmoves = 0;
    std::size_t m_fullmoves = 1;
    Colour m_turn = Colour::White;
    std::array<Square, 4> m_castle_rooks = {Square::H1, Square::A1, Square::H8, Square::A8};
    int m_castling = 0;
    Square m_enpassant = Square::None;
    zobrist::hash_type m_hash = 0;
    std::vector<History> m_history;
};

inline auto operator<<(std::ostream &os, const Position &pos) noexcept -> std::ostream & {
    for (int y = 7; y >= 0; --y) {
        for (int x = 0; x < 8; ++x) {
            const auto sq = Square(y * 8 + x);
            const auto bb = Bitboard(sq);

            if (bb & pos.get_pawns()) {
                if (bb & pos.get_white()) {
                    os << 'P';
                } else {
                    os << 'p';
                }
            } else if (bb & pos.get_knights()) {
                if (bb & pos.get_white()) {
                    os << 'N';
                } else {
                    os << 'n';
                }
            } else if (bb & pos.get_bishops()) {
                if (bb & pos.get_white()) {
                    os << 'B';
                } else {
                    os << 'b';
                }
            } else if (bb & pos.get_rooks()) {
                if (bb & pos.get_white()) {
                    os << 'R';
                } else {
                    os << 'r';
                }
            } else if (bb & pos.get_queens()) {
                if (bb & pos.get_white()) {
                    os << 'Q';
                } else {
                    os << 'q';
                }
            } else if (bb & pos.get_kings()) {
                if (bb & pos.get_white()) {
                    os << 'K';
                } else {
                    os << 'k';
                }
            } else {
                os << '-';
            }
        }
        os << '\n';
    }
    os << "Halfmoves: " << pos.halfmoves() << "\n";
    os << "Fullmoves: " << pos.fullmoves() << "\n";
    os << "Castling: ";
    if (pos.can_castle(CastleType::WhiteKingSide)) {
        os << 'K';
    }
    if (pos.can_castle(CastleType::WhiteQueenSide)) {
        os << 'Q';
    }
    if (pos.can_castle(CastleType::BlackKingSide)) {
        os << 'k';
    }
    if (pos.can_castle(CastleType::BlackQueenSide)) {
        os << 'q';
    }
    os << '\n';
    os << "EnPassant: " << sq_strings[index(pos.enpassant())] << "\n";
    os << "Checked: " << pos.is_attacked(pos.get_king(pos.turn()), !pos.turn()) << "\n";
    os << "Turn: " << (pos.turn() == Colour::White ? "white" : "black") << "\n";
    return os;
}

}  // namespace chess

#endif

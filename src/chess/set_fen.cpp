#include <sstream>
#include <vector>
#include "position.hpp"

namespace chess {

[[nodiscard]] auto split(const std::string_view str, const std::string_view delims = " ")
    -> std::vector<std::string_view> {
    std::vector<std::string_view> output;

    for (auto first = str.data(), second = str.data(), last = first + str.size(); second != last && first != last;
         first = second + 1) {
        second = std::find_first_of(first, last, std::cbegin(delims), std::cend(delims));

        if (first != second) {
            output.emplace_back(first, second - first);
        }
    }

    return output;
}

auto Position::set_fen(const std::string_view fen) -> void {
    if (fen == "startpos") {
        set_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        return;
    }

    // Reset
    m_colour.fill(Bitboard());
    m_piece.fill(Bitboard());
    m_halfmoves = 0;
    m_fullmoves = 1;
    m_turn = Colour::White;
    m_castle_rooks = {Square::H1, Square::A1, Square::H8, Square::A8};
    m_castling = 0;
    m_enpassant = Square::None;
    m_hash = 0;
    m_history.clear();

    const auto parts = split(fen, " ");

    // Pieces
    if (parts.size() >= 1) {
        int i = 56;
        for (const auto &c : parts[0]) {
            const auto sq = Square(i);

            switch (c) {
                case 'P':
                    m_colour[index(Colour::White)] ^= Bitboard(sq);
                    m_piece[index(PieceType::Pawn)] ^= Bitboard(sq);
                    i++;
                    break;
                case 'p':
                    m_colour[index(Colour::Black)] ^= Bitboard(sq);
                    m_piece[index(PieceType::Pawn)] ^= Bitboard(sq);
                    i++;
                    break;
                case 'N':
                    m_colour[index(Colour::White)] ^= Bitboard(sq);
                    m_piece[index(PieceType::Knight)] ^= Bitboard(sq);
                    i++;
                    break;
                case 'n':
                    m_colour[index(Colour::Black)] ^= Bitboard(sq);
                    m_piece[index(PieceType::Knight)] ^= Bitboard(sq);
                    i++;
                    break;
                case 'B':
                    m_colour[index(Colour::White)] ^= Bitboard(sq);
                    m_piece[index(PieceType::Bishop)] ^= Bitboard(sq);
                    i++;
                    break;
                case 'b':
                    m_colour[index(Colour::Black)] ^= Bitboard(sq);
                    m_piece[index(PieceType::Bishop)] ^= Bitboard(sq);
                    i++;
                    break;
                case 'R':
                    m_colour[index(Colour::White)] ^= Bitboard(sq);
                    m_piece[index(PieceType::Rook)] ^= Bitboard(sq);
                    i++;
                    break;
                case 'r':
                    m_colour[index(Colour::Black)] ^= Bitboard(sq);
                    m_piece[index(PieceType::Rook)] ^= Bitboard(sq);
                    i++;
                    break;
                case 'Q':
                    m_colour[index(Colour::White)] ^= Bitboard(sq);
                    m_piece[index(PieceType::Queen)] ^= Bitboard(sq);
                    i++;
                    break;
                case 'q':
                    m_colour[index(Colour::Black)] ^= Bitboard(sq);
                    m_piece[index(PieceType::Queen)] ^= Bitboard(sq);
                    i++;
                    break;
                case 'K':
                    m_colour[index(Colour::White)] ^= Bitboard(sq);
                    m_piece[index(PieceType::King)] ^= Bitboard(sq);
                    i++;
                    break;
                case 'k':
                    m_colour[index(Colour::Black)] ^= Bitboard(sq);
                    m_piece[index(PieceType::King)] ^= Bitboard(sq);
                    i++;
                    break;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                    i += c - '1' + 1;
                    break;
                case '/':
                    i -= 16;
                    break;
                default:
                    break;
            }
        }
    }

    // Side to move
    if (parts.size() >= 2) {
        if (parts[1] == "w") {
            m_turn = Colour::White;
        } else {
            m_turn = Colour::Black;
        }
    }

    // Castling
    if (parts.size() >= 3 && parts[2] != "-") {
        auto get_square = [](const char c) noexcept -> Square {
            if (c == 'K') {
                return Square::H1;
            } else if (c == 'Q') {
                return Square::A1;
            } else if (c == 'k') {
                return Square::H8;
            } else if (c == 'q') {
                return Square::A8;
            } else if ('A' <= c && c <= 'H') {
                return Square(c - 'A');
            } else if ('a' <= c && c <= 'h') {
                return Square(56 + c - 'a');
            } else {
                return Square::None;
            }
        };

        auto is_uppercase = [](const char c) noexcept -> bool {
            return 'A' <= c && c <= 'Z';
        };

        const auto wksq = get_king(Colour::White);
        const auto bksq = get_king(Colour::Black);
        for (const auto c : parts[2]) {
            const auto perm_square = get_square(c);
            if (perm_square == Square::None) {
                break;
            }
            const auto is_white = is_uppercase(c);
            const auto rooks = get_rooks() & Bitboard::rank_of(perm_square);
            const auto ksq = is_white ? wksq : bksq;
            const auto is_ksc = file(perm_square) > file(ksq);
            const auto rook_found = rooks.is_occupied(perm_square);
            const auto rook_east = rook_found ? perm_square : rooks.hsb();
            const auto rook_west = rook_found ? perm_square : rooks.lsb();

            if (is_white) {
                if (is_ksc) {
                    m_castling |= (1 << static_cast<int>(CastleType::WhiteKingSide));
                    m_castle_rooks[0] = rook_east;
                } else {
                    m_castling |= (1 << static_cast<int>(CastleType::WhiteQueenSide));
                    m_castle_rooks[1] = rook_west;
                }
            } else {
                if (is_ksc) {
                    m_castling |= (1 << static_cast<int>(CastleType::BlackKingSide));
                    m_castle_rooks[2] = rook_east;
                } else {
                    m_castling |= (1 << static_cast<int>(CastleType::BlackQueenSide));
                    m_castle_rooks[3] = rook_west;
                }
            }
        }
    }

    // En passant
    if (parts.size() >= 3 && parts[3].size() == 2) {
        const auto file = parts[3][0] - 'a';
        const auto rank = parts[3][1] - '1';
        m_enpassant = Square(rank * 8 + file);
    }

    // Halfmoves
    if (parts.size() >= 4) {
        m_halfmoves = std::stoi(std::string(parts[4]));
    }

    // Fullmoves
    if (parts.size() >= 5) {
        m_fullmoves = std::stoi(std::string(parts[5]));
    }

    m_hash = calculate_hash();
}

}  // namespace chess

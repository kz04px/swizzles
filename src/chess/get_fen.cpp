#include "position.hpp"

namespace chess {

auto Position::get_fen(const bool chess960) const noexcept -> std::string {
    std::string fen;

    // Pieces
    for (int y = 7; y >= 0; --y) {
        int empty = 0;
        for (int x = 0; x < 8; ++x) {
            const auto sq = Square(8 * y + x);
            const auto bb = Bitboard(sq);

            if (is_empty(sq)) {
                empty++;
                continue;
            }

            if (empty > 0) {
                fen += std::to_string(empty);
                empty = 0;
            }

            if (bb & get_pawns()) {
                if (bb & get_white()) {
                    fen += 'P';
                } else {
                    fen += 'p';
                }
            } else if (bb & get_knights()) {
                if (bb & get_white()) {
                    fen += 'N';
                } else {
                    fen += 'n';
                }
            } else if (bb & get_bishops()) {
                if (bb & get_white()) {
                    fen += 'B';
                } else {
                    fen += 'b';
                }
            } else if (bb & get_rooks()) {
                if (bb & get_white()) {
                    fen += 'R';
                } else {
                    fen += 'r';
                }
            } else if (bb & get_queens()) {
                if (bb & get_white()) {
                    fen += 'Q';
                } else {
                    fen += 'q';
                }
            } else if (bb & get_kings()) {
                if (bb & get_white()) {
                    fen += 'K';
                } else {
                    fen += 'k';
                }
            }
        }

        if (empty > 0) {
            fen += std::to_string(empty);
        }

        if (y > 0) {
            fen += "/";
        }
    }

    // Sude to move
    if (m_turn == Colour::White) {
        fen += " w";
    } else {
        fen += " b";
    }

    // Castling
    if (chess960) {
        std::string part;

        if (can_castle(CastleType::WhiteKingSide)) {
            part += 'A' + static_cast<char>(file(m_castle_rooks[0]));
        }
        if (can_castle(CastleType::WhiteQueenSide)) {
            part += 'A' + static_cast<char>(file(m_castle_rooks[1]));
        }
        if (can_castle(CastleType::BlackKingSide)) {
            part += 'a' + static_cast<char>(file(m_castle_rooks[2]));
        }
        if (can_castle(CastleType::BlackQueenSide)) {
            part += 'a' + static_cast<char>(file(m_castle_rooks[3]));
        }

        if (part.empty()) {
            fen += " -";
        } else {
            fen += " " + part;
        }
    } else {
        std::string part;

        if (can_castle(CastleType::WhiteKingSide)) {
            part += "K";
        }
        if (can_castle(CastleType::WhiteQueenSide)) {
            part += "Q";
        }
        if (can_castle(CastleType::BlackKingSide)) {
            part += "k";
        }
        if (can_castle(CastleType::BlackQueenSide)) {
            part += "q";
        }

        if (part.empty()) {
            fen += " -";
        } else {
            fen += " " + part;
        }
    }

    // Enpassant
    if (m_enpassant == Square::None) {
        fen += " -";
    } else {
        fen += " ";
        fen += 'a' + static_cast<char>(file(m_enpassant));
        fen += '1' + static_cast<char>(rank(m_enpassant));
    }

    // Halfmoves
    fen += " " + std::to_string(m_halfmoves);

    // Fullmoves
    fen += " " + std::to_string(m_fullmoves);

    return fen;
}

}  // namespace chess

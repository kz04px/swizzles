#include "position.hpp"
#include "zobrist.hpp"

namespace chess {

[[nodiscard]] auto Position::calculate_hash() const noexcept -> zobrist::hash_type {
    zobrist::hash_type hash = 0;

    if (turn() == Colour::Black) {
        hash ^= zobrist::turn_key();
    }

    for (const auto sq : get_white()) {
        const auto piece = piece_on(sq);
        hash ^= zobrist::piece_key(piece, Colour::White, sq);
    }

    for (const auto sq : get_black()) {
        const auto piece = piece_on(sq);
        hash ^= zobrist::piece_key(piece, Colour::Black, sq);
    }

    if (m_enpassant != Square::None) {
        hash ^= zobrist::ep_key(m_enpassant);
    }

    hash ^= zobrist::castling_key(m_castling);

    return hash;
}

}  // namespace chess

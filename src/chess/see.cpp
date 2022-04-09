#include "see.hpp"
#include "colour.hpp"
#include "magic.hpp"
#include "move.hpp"
#include "position.hpp"
#include "square.hpp"

namespace chess {

namespace attacks {

[[nodiscard]] auto pawn(const Square sq, const Colour us) noexcept -> Bitboard {
    if (us == Colour::White) {
        return Bitboard(sq).northwest() | Bitboard(sq).northeast();
    } else {
        return Bitboard(sq).southwest() | Bitboard(sq).southeast();
    }
}

[[nodiscard]] auto knight(const Square sq) noexcept -> Bitboard {
    return Bitboard(sq).knight();
}

[[nodiscard]] auto bishop(const Square sq, const Bitboard blockers) noexcept -> Bitboard {
    return magic::bishop_moves(sq, blockers);
}

[[nodiscard]] auto rook(const Square sq, const Bitboard blockers) noexcept -> Bitboard {
    return magic::rook_moves(sq, blockers);
}

[[nodiscard]] auto queen(const Square sq, const Bitboard blockers) noexcept -> Bitboard {
    return magic::queen_moves(sq, blockers);
}

[[nodiscard]] auto king(const Square sq) noexcept -> Bitboard {
    return Bitboard(sq).adjacent();
}

}  // namespace attacks

[[nodiscard]] static auto get_smallest_attacker(const Square sq,
                                                const Colour us,
                                                std::array<Bitboard, 2> &colours,
                                                std::array<Bitboard, 6> &pieces) noexcept
    -> std::pair<chess::PieceType, chess::Square> {
    const auto blockers = colours[0] | colours[1];
    Bitboard attackers;

    // Pawns
    if ((attackers = colours[index(us)] & attacks::pawn(sq, !us) & pieces[index(PieceType::Pawn)])) {
        return {PieceType::Pawn, attackers.lsb()};
    }
    // Knights
    else if ((attackers = colours[index(us)] & pieces[index(PieceType::Knight)] & Bitboard(sq).knight())) {
        return {PieceType::Knight, attackers.lsb()};
    }
    // Bishops
    else if ((attackers = colours[index(us)] & pieces[index(PieceType::Bishop)] & magic::bishop_moves(sq, blockers))) {
        return {PieceType::Bishop, attackers.lsb()};
    }
    // Rooks
    else if ((attackers = colours[index(us)] & pieces[index(PieceType::Rook)] & magic::rook_moves(sq, blockers))) {
        return {PieceType::Rook, attackers.lsb()};
    }
    // Queens
    else if ((attackers = colours[index(us)] & pieces[index(PieceType::Queen)] & magic::queen_moves(sq, blockers))) {
        return {PieceType::Queen, attackers.lsb()};
    }
    // Kings
    else if ((attackers = colours[index(us)] & pieces[index(PieceType::King)] & Bitboard(sq).adjacent())) {
        return {PieceType::King, attackers.lsb()};
    }
    // skip if the square isn't attacked by us anymore
    else {
        return {PieceType::None, Square::None};
    }
}

[[nodiscard]] static auto see(const Square sq,
                              const Colour us,
                              const PieceType captured,
                              std::array<Bitboard, 2> &colours,
                              std::array<Bitboard, 6> &pieces,
                              const std::array<int, 7> &values) noexcept -> int {
    const auto [smallest_attacker, from_sq] = get_smallest_attacker(sq, us, colours, pieces);

    if (smallest_attacker == PieceType::None) {
        return 0;
    }

    // Make move
    colours[index(us)] ^= Bitboard(from_sq);
    pieces[index(smallest_attacker)] ^= Bitboard(from_sq);

    const auto value = values[index(captured)] - see(sq, !us, smallest_attacker, colours, pieces, values);

    // Undo move
    colours[index(us)] ^= Bitboard(from_sq);
    pieces[index(smallest_attacker)] ^= Bitboard(from_sq);

    return std::max(value, 0);
}

[[nodiscard]] auto see_move(const Position &pos, const Move &move, const std::array<int, 7> &values) noexcept -> int {
    const auto captured = pos.piece_on(move.to());
    auto colours = pos.get_colours();
    auto pieces = pos.get_pieces();

    // Make move
    colours[index(pos.turn())] ^= Bitboard(move.from());
    pieces[index(pos.turn())] ^= Bitboard(move.from());

    return values[index(captured)] - see(move.to(), !pos.turn(), move.piece(), colours, pieces, values);
}

}  // namespace chess

#include "see.hpp"
#include "../assert.hpp"
#include "../chess/bitboard.hpp"
#include "../chess/movegen.hpp"
#include "../chess/piece.hpp"

namespace search {

static constexpr int values[] = {100, 320, 330, 500, 900, 1000000};

template <chess::Side side>
[[nodiscard]] std::pair<chess::Piece, chess::Square> smallest_attacker(chess::Bitboard colours[2],
                                                                       chess::Bitboard pieces[6],
                                                                       const chess::Square sq) {
    const auto bb = chess::Bitboard(sq);
    const auto blockers = colours[0] | colours[1];

    {
        // Pawns
        const auto attackers =
            (bb.forwards<!side>().east() | bb.forwards<!side>().west()) & pieces[0] & colours[static_cast<int>(side)];
        if (attackers) {
            return {chess::Piece::Pawn, attackers.lsbll()};
        }
    }

    {
        // Knights
        const auto attackers = chess::movegen::knight_moves(sq) & pieces[1] & colours[static_cast<int>(side)];
        if (attackers) {
            return {chess::Piece::Knight, attackers.lsbll()};
        }
    }

    {
        // Bishops
        const auto attackers = chess::movegen::bishop_moves(sq, blockers) & pieces[2] & colours[static_cast<int>(side)];
        if (attackers) {
            return {chess::Piece::Bishop, attackers.lsbll()};
        }
    }

    {
        // Rooks
        const auto attackers = chess::movegen::rook_moves(sq, blockers) & pieces[3] & colours[static_cast<int>(side)];
        if (attackers) {
            return {chess::Piece::Rook, attackers.lsbll()};
        }
    }

    {
        // Queens
        const auto attackers = chess::movegen::queen_moves(sq, blockers) & pieces[4] & colours[static_cast<int>(side)];
        if (attackers) {
            return {chess::Piece::Queen, attackers.lsbll()};
        }
    }

    {
        // King
        const auto attackers = chess::movegen::king_moves(sq) & pieces[5] & colours[static_cast<int>(side)];
        if (attackers) {
            return {chess::Piece::King, attackers.lsbll()};
        }
    }

    return {chess::Piece::None, {}};
}

[[nodiscard]] chess::Piece piece_on(const chess::Bitboard pieces[6], const chess::Square sq) {
    const auto bb = chess::Bitboard(sq);

    for (int i = 0; i < 6; ++i) {
        if (pieces[i] & bb) {
            return static_cast<chess::Piece>(i);
        }
    }

    return chess::Piece::None;
}

template <chess::Side side>
[[nodiscard]] int SEE(chess::Bitboard colours[2], chess::Bitboard pieces[6], const chess::Square sq) {
    UCI_ASSERT(piece_on(pieces, sq) != chess::Piece::None);

    const auto [p, psq] = smallest_attacker<side>(colours, pieces, sq);

    if (p == chess::Piece::None) {
        return 0;
    }

    int value = 0;
    const auto pidx = static_cast<int>(p);
    const auto captured = piece_on(pieces, sq);
    const auto captured_idx = static_cast<int>(captured);

    UCI_ASSERT(pidx < 6);
    UCI_ASSERT(colours[static_cast<int>(side)] & chess::Bitboard(psq));
    UCI_ASSERT(pieces[pidx] & chess::Bitboard(psq));
    UCI_ASSERT(captured != chess::Piece::None);
    UCI_ASSERT(psq != sq);

    // Remove our piece
    colours[static_cast<int>(side)] ^= chess::Bitboard(psq);
    pieces[pidx] ^= chess::Bitboard(psq);

    // Add our piece
    colours[static_cast<int>(side)] ^= chess::Bitboard(sq);
    pieces[pidx] ^= chess::Bitboard(sq);

    // Remove their piece
    colours[static_cast<int>(!side)] ^= chess::Bitboard(sq);
    pieces[captured_idx] ^= chess::Bitboard(sq);

    value = std::max(0, values[captured_idx] - SEE<!side>(colours, pieces, sq));

    // Undo
    colours[static_cast<int>(side)] ^= chess::Bitboard(psq);
    pieces[pidx] ^= chess::Bitboard(psq);

    // Undo
    colours[static_cast<int>(side)] ^= chess::Bitboard(sq);
    pieces[pidx] ^= chess::Bitboard(sq);

    // Undo
    colours[static_cast<int>(!side)] ^= chess::Bitboard(sq);
    pieces[captured_idx] ^= chess::Bitboard(sq);

    return value;
}

[[nodiscard]] int SEEcapture(chess::Position &pos, const chess::Move &move) {
    UCI_ASSERT(pos.is_legal(move));
    UCI_ASSERT(move.is_capturing());

    pos.makemove(move);

    chess::Bitboard colours[2] = {pos.white(), pos.black()};
    chess::Bitboard pieces[6] = {pos.piece(chess::Piece::Pawn),
                                 pos.piece(chess::Piece::Knight),
                                 pos.piece(chess::Piece::Bishop),
                                 pos.piece(chess::Piece::Rook),
                                 pos.piece(chess::Piece::Queen),
                                 pos.piece(chess::Piece::King)};

    pos.undomove();

    if (pos.turn() == chess::Side::White) {
        return values[static_cast<int>(move.captured())] - SEE<chess::Side::Black>(colours, pieces, move.to());
    } else {
        return values[static_cast<int>(move.captured())] - SEE<chess::Side::White>(colours, pieces, move.to());
    }
}

[[nodiscard]] int SEEquiet(chess::Position &pos, const chess::Move &move) {
    UCI_ASSERT(pos.is_legal(move));
    UCI_ASSERT(!move.is_capturing());

    pos.makemove(move);

    chess::Bitboard colours[2] = {pos.white(), pos.black()};
    chess::Bitboard pieces[6] = {pos.piece(chess::Piece::Pawn),
                                 pos.piece(chess::Piece::Knight),
                                 pos.piece(chess::Piece::Bishop),
                                 pos.piece(chess::Piece::Rook),
                                 pos.piece(chess::Piece::Queen),
                                 pos.piece(chess::Piece::King)};

    pos.undomove();

    if (pos.turn() == chess::Side::White) {
        return 0 - SEE<chess::Side::Black>(colours, pieces, move.to());
    } else {
        return 0 - SEE<chess::Side::White>(colours, pieces, move.to());
    }
}

}  // namespace search

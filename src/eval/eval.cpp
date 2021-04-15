#include "eval.hpp"
#include <limits>
#include "../assert.hpp"
#include "../chess/bitboard.hpp"
#include "../score.hpp"
#include "../search/constants.hpp"
#include "king_safety.hpp"
#include "mobility.hpp"
#include "phase.hpp"
#include "pst.hpp"
#include "values.hpp"

using namespace eval::values;

namespace eval {

template <chess::Side side>
[[nodiscard]] Score eval_side(const chess::Position &pos) {
    Score score;

    // Piece pairs
    score += knight_pair * pos.knights<side>().many();
    score += bishop_pair * pos.bishops<side>().many();

    // King safety
    score += king_safety<side>(pos);

    // Mobility
    score += mobility<side>(pos);

    // For each file
    for (int i = 0; i < 8; ++i) {
        const auto file = chess::bitboards::files[i];
        const auto adjacent_files = chess::bitboards::adjacent_files[i];
        const auto pawns = pos.piece(chess::Piece::Pawn) & file;
        const auto our_rooks = pos.rooks<side>() & file;
        const auto our_queens = pos.queens<side>() & file;
        const auto open_file = !pawns;

        // Doubled pawns
        if (pawns.many()) {
            score += doubled_pawns;
        }

        // Isolated pawns
        if (pawns && !(adjacent_files & pos.pawns<side>())) {
            score += isolated_pawn;
        }

        // Rooks & Queens on open files
        if (open_file && (our_rooks | our_queens)) {
            score += rook_open_file;
        }
    }

    // Pawns - chains
    {
        const chess::Bitboard pawns = pos.pawns<side>();
        const auto boys = pawns & (pawns.forwards<side>().east() | pawns.forwards<side>().west());
        score += pawn_chain * boys.count();
    }

    // Every piece
    for (const auto p : chess::pieces) {
        // Piece values
        score += piece_values[static_cast<int>(p)] * pos.pieces(side, p).count();

        // PST
        for (const auto sq : pos.pieces(side, p)) {
            score += pst_value<side>(p, sq);
        }
    }

    return score;
}

[[nodiscard]] int eval(const chess::Position &pos) {
    Score score;
    score += eval_side<chess::Side::White>(pos);
    score -= eval_side<chess::Side::Black>(pos);

    // Side to move
    score = pos.turn() == chess::Side::White ? score : -score;

    // Turn bonus
    score += turn_bonus;

    const auto phased = phase(pos, score);

    UCI_ASSERT(phased <= constants::max_eval);

    return phased;
}

}  // namespace eval

#include "qsearch.hpp"
#include "../assert.hpp"
#include "../eval/eval.hpp"
#include "../search/see.hpp"

namespace search {

[[nodiscard]] int qsearch(chess::Position &pos, int alpha, const int beta) {
    const auto stand_pat = eval::eval(pos);

    if (stand_pat >= beta) {
        return beta;
    }

    if (alpha < stand_pat) {
        alpha = stand_pat;
    }

    auto moves = pos.legal_captures();

    UCI_ASSERT(moves.size() <= 256);

    if (moves.empty()) {
        return alpha;
    }

    // Score moves
    int scores[moves.size()];
    for (std::size_t i = 0; i < moves.size(); ++i) {
        scores[i] = SEEcapture(pos, moves[i]);
    }

    // Sort moves
    for (std::size_t i = 0; i < moves.size() - 1; ++i) {
        std::size_t idx = i;

        for (std::size_t j = i + 1; j < moves.size(); ++j) {
            if (scores[j] > scores[idx]) {
                idx = j;
            }
        }

        std::swap(scores[i], scores[idx]);
        std::swap(moves[i], moves[idx]);
    }

    /*
    for (std::size_t i = 0; i < moves.size() - 1; ++i) {
        UCI_ASSERT(scores[i] >= scores[i + 1]);
    }
    */

    for (std::size_t i = 0; i < moves.size(); ++i) {
        if (scores[i] < -50) {
            break;
        }

        pos.makemove(moves[i]);
        const auto score = -qsearch(pos, -beta, -alpha);
        pos.undomove();

        if (score >= beta) {
            return beta;
        }

        if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}

}  // namespace search

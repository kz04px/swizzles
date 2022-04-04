#include <chess/position.hpp>
#include "../eval/eval.hpp"
#include "search.hpp"
#include "sort.hpp"

namespace swizzles::search {

[[nodiscard]] auto qsearch(chess::Position &pos, int alpha, const int beta, const ThreadData &td) noexcept -> int {
    const auto stand_pat = eval::eval(pos);

    if (stand_pat >= beta) {
        return beta;
    }

    if (alpha < stand_pat) {
        alpha = stand_pat;
    }

    auto moves = pos.captures();
    sort(moves, chess::Move(), td, pos.turn());

    for (const auto move : moves) {
        pos.makemove(move);
        const auto score = -qsearch(pos, -beta, -alpha, td);
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

}  // namespace swizzles::search

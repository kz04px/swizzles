#include <chess/position.hpp>
#include "../eval/eval.hpp"
#include "search.hpp"
#include "sort.hpp"

namespace swizzles::search {

[[nodiscard]] auto qsearch(const ThreadData &td, chess::Position &pos, int alpha, const int beta) noexcept -> int {
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

        if (pos.is_attacked(pos.get_king(!pos.turn()), pos.turn())) {
            pos.undomove();
            continue;
        }

        const auto score = -qsearch(td, pos, -beta, -alpha);

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

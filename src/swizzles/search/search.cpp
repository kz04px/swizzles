#include "search.hpp"
#include <chess/position.hpp>
#include <limits>
#include "../eval/eval.hpp"

namespace swizzles::search {

static constexpr int mate_score = 1'000'000;

[[nodiscard]] auto search(ThreadData &td, SearchStack *ss, chess::Position &pos, const int depth) noexcept -> int {
    td.seldepth = std::max(td.seldepth, ss->ply);

    const auto is_root = ss->ply == 0;
    const auto in_check = pos.is_attacked(pos.get_kings(pos.turn()), !pos.turn());

    if (depth == 0 || ss->ply == 127) {
        return eval::eval(pos);
    }

    if (pos.halfmoves() >= 100) {
        return 0;
    }

    if (const auto repeats = pos.num_repeats(); (is_root && repeats == 3) || (!is_root && repeats == 2)) {
        return 0;
    }

    td.controller->update();

    if (td.controller->should_stop()) {
        return 0;
    }

    auto best_score = std::numeric_limits<int>::min();

    const auto moves = pos.movegen();
    for (const auto &move : moves) {
        pos.makemove(move);

        if (pos.is_attacked(pos.get_king(!pos.turn()), pos.turn())) {
            pos.undomove();
            continue;
        }

        td.nodes++;

        const auto score = -search(td, ss + 1, pos, depth - 1);
        pos.undomove();

        if (score > best_score) {
            best_score = score;
            ss->pv.clear();
            ss->pv.push_back(move);
        }
    }

    if (best_score == std::numeric_limits<int>::min()) {
        if (in_check) {
            return -mate_score + ss->ply;
        } else {
            return 0;
        }
    }

    return best_score;
}

}  // namespace swizzles::search

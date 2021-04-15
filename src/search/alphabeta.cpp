#include "alphabeta.hpp"
#include <cassert>
#include <limits>
#include "../assert.hpp"
#include "../eval/eval.hpp"
#include "../globals.hpp"
#include "../sort.hpp"
#include "../tt.hpp"
#include "../ttentry.hpp"
#include "constants.hpp"
#include "controller.hpp"
#include "is_endgame.hpp"
#include "qsearch.hpp"
#include "reduction.hpp"
#include "see.hpp"

namespace search {

static constexpr int futility_margins[] = {0, 330, 500, 900};

int alphabeta(chess::Position &pos, Stack *ss, int alpha, int beta, int depth) {
    UCI_ASSERT(ss);
    UCI_ASSERT(0 <= ss->ply && ss->ply < constants::max_depth);
    UCI_ASSERT(-constants::mate_score <= alpha);
    UCI_ASSERT(alpha < beta);
    UCI_ASSERT(beta <= constants::mate_score);

    ss->pv.clear();
    const auto in_check = pos.in_check();
    const auto pvnode = beta - alpha > 1;
    const auto root = ss->ply == 0;
    const auto alpha_orig = alpha;

    UCI_ASSERT(!root || (root && pvnode));

    if (pos.threefold()) {
        return 0;
    }

    if (pos.fiftymoves()) {
        // Checkmate takes priority over the fifty move rule
        if (in_check) {
            return pos.legal_moves().size() == 0 ? -constants::mate_score + ss->ply : 0;
        }
        return 0;
    }

    // Check extension
    if (in_check) {
        depth++;
    }

    if (depth <= 0 || ss->ply == constants::max_depth) {
        UCI_ASSERT(!root);
        return qsearch(pos, alpha, beta);
    }

    if (controller.should_stop()) {
        return 0;
    }

    // Poll TT
    chess::Move ttmove;
    const auto ttentry = tt.poll(pos.hash());
    if (pos.hash() == ttentry.hash) {
        ttmove = ttentry.move;
        const auto ttscore = eval_from_tt(ttentry.score, ss->ply);

        if (!pvnode && ttentry.depth >= depth) {
            switch (ttentry.flag) {
                case Flag::Exact:
                    ss->pv.push_back(ttentry.move);
                    return ttscore;
                case Flag::Lower:
                    alpha = std::max(alpha, ttscore);
                    break;
                case Flag::Upper:
                    beta = std::min(beta, ttscore);
                    break;
            }

            if (alpha >= beta) {
                ss->pv.push_back(ttentry.move);
                return ttscore;
            }
        }
    }

    const int static_eval = eval::eval(pos);

    // Reverse futility pruning
    if (!pvnode && ss->nullmove && depth <= 3 && !in_check && !is_endgame(pos) &&
        std::abs(beta) <= constants::max_eval && static_eval - futility_margins[depth] >= beta) {
        UCI_ASSERT(!root);
        return static_eval - futility_margins[depth];
    }

    // Null Move Pruning
    if (!pvnode && ss->nullmove && depth > 2 && !in_check && !is_endgame(pos)) {
        UCI_ASSERT(!root);

        pos.makenull();
        (ss + 1)->nullmove = false;

        const int score = -alphabeta(pos, ss + 1, -beta, -beta + 1, depth - 3);

        pos.undonull();
        (ss + 1)->nullmove = true;

        if (score >= beta) {
            // ss->pv.clear();
            return score;
        }
    }

    auto best_score = std::numeric_limits<int>::min();
    auto moves = pos.legal_moves();

    if (moves.empty()) {
        // Checkmate
        if (in_check) {
            return -constants::mate_score + ss->ply;
        }
        // Stalemate
        else {
            return 0;
        }
    }

    // Move sorting
    sort(moves, [&pos, ttmove, ss](const chess::Move move) {
        if (move == ttmove) {
            return 100000000;
        } else if (move == ss->killer_1) {
            return 5000000;
        } else if (move.is_capturing()) {
            return 5000000 + SEEcapture(pos, move);
        } else {
            return SEEquiet(pos, move);
        }
    });

    chess::Move best_move;
    for (std::size_t i = 0; i < moves.size(); ++i) {
        controller.increment_nodes();

        pos.makemove(moves[i]);

        int score;
        if (i == 0) {
            score = -alphabeta(pos, ss + 1, -beta, -alpha, depth - 1);
        } else {
            const auto r = reduction(i, depth, in_check, moves[i].type());
            score = -alphabeta(pos, ss + 1, -alpha - 1, -alpha, depth - 1 - r);

            // Re-search
            if (alpha < score && score < beta) {
                score = -alphabeta(pos, ss + 1, -beta, -alpha, depth - 1);
            }
        }

        pos.undomove();

        if (score > best_score) {
            best_score = score;
            best_move = moves[i];
            ss->pv.clear();
            ss->pv.push_back(moves[i]);
            for (const auto &m : (ss + 1)->pv) {
                ss->pv.push_back(m);
            }
        }

        if (score > alpha) {
            alpha = score;

            if (score >= beta) {
                // Store killer move
                if (!moves[i].is_capturing()) {
                    ss->killer_1 = moves[i];
                }

                break;
            }
        }
    }

    UCI_ASSERT(best_score > std::numeric_limits<int>::min());
    UCI_ASSERT(pos.is_legal(best_move));
    UCI_ASSERT(!ss->pv.empty());
    UCI_ASSERT(legal_pv(pos, ss->pv));

    TTEntry entry;
    entry.hash = pos.hash();
    entry.move = best_move;
    entry.score = eval_to_tt(best_score, ss->ply);
    entry.depth = depth;
    if (best_score <= alpha_orig) {
        entry.flag = Flag::Upper;
    } else if (best_score >= beta) {
        entry.flag = Flag::Lower;
    } else {
        entry.flag = Flag::Exact;
    }
    tt.add(pos.hash(), entry);

    return best_score;
}

}  // namespace search

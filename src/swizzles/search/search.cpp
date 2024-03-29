#include "search.hpp"
#include <chess/position.hpp>
#include <limits>
#include <tt.hpp>
#include "../eval/eval.hpp"
#include "../ttentry.hpp"
#include "qsearch.hpp"
#include "sort.hpp"

namespace swizzles::search {

[[nodiscard]] auto is_endgame(const chess::Position &pos) noexcept -> bool {
    const auto piece_mask = pos.get_knights() | pos.get_bishops() | pos.get_rooks() | pos.get_queens();
    return (piece_mask & pos.colour(pos.turn())).count() <= 2;
}

[[nodiscard]] auto reduction(const chess::Move move,
                             const int depth,
                             const int legal_moves,
                             const bool in_check,
                             const bool gives_check,
                             const bool is_root) noexcept -> int {
    if (!is_root && !in_check && legal_moves > 4 && depth >= 3 && move.promo() == chess::PieceType::None &&
        move.captured() == chess::PieceType::None && !gives_check) {
        auto r = 1;
        if (depth >= 6) {
            r = 2;
        }
        if (legal_moves >= 20) {
            r += 1;
        }
        return r;
    }
    return 0;
}

[[nodiscard]] auto search(ThreadData &td,
                          SearchStack *ss,
                          chess::Position &pos,
                          int alpha,
                          int beta,
                          int depth) noexcept -> int {
    td.seldepth = std::max(td.seldepth, ss->ply);
    const auto alpha_orig = alpha;

    const auto ttentry = td.tt->poll(pos.hash());
    if (ttentry.hash == pos.hash() && ttentry.depth >= depth) {
        const auto eval = eval_from_tt(ttentry.eval, ss->ply);

        if (ttentry.flag == TTFlag::Exact) {
            ss->pv.clear();
            ss->pv.push_back(ttentry.move);
            return eval;
        } else if (ttentry.flag == TTFlag::Lower) {
            alpha = std::max(alpha, eval);
        } else if (ttentry.flag == TTFlag::Upper) {
            beta = std::min(beta, eval);
        }

        if (alpha >= beta) {
            ss->pv.clear();
            ss->pv.push_back(ttentry.move);
            return eval;
        }
    }

    const auto is_root = ss->ply == 0;
    const auto in_check = pos.is_attacked(pos.get_kings(pos.turn()), !pos.turn());

    if (in_check) {
        depth++;
    }

    if (depth == 0 || ss->ply == max_depth) {
        return qsearch(td, pos, alpha, beta);
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

    // Static Null Move Pruning
    if (!ss->null_move && !is_root && std::abs(beta) <= mate_score - max_depth) {
        const auto static_eval = eval::eval(pos);

        if (depth == 1 && static_eval - 300 > beta) {
            return beta;
        } else if (depth == 2 && static_eval - 500 > beta) {
            return beta;
        } else if (depth == 3 && static_eval - 900 > beta) {
            depth--;
        }
    }

    // Null Move Pruning
    if (!ss->null_move && depth >= 3 && !in_check && !is_root && !is_endgame(pos)) {
        pos.makenull();

        (ss + 1)->null_move = true;
        const auto score = -search(td, ss + 1, pos, -beta, -beta + 1, depth - 1 - 2);
        (ss + 1)->null_move = false;

        pos.undonull();

        ss->pv.clear();

        if (score >= beta) {
            return score;
        }
    }

    auto legal_moves = 0;
    auto best_score = std::numeric_limits<int>::min();
    auto best_move = chess::Move();
    auto moves = pos.movegen();

    sort(moves, ttentry.move, td, pos.turn());

    // Prob cut
    if (!is_root && depth >= 5 && std::abs(beta) < mate_score - max_depth) {
        const auto r_beta = std::min(mate_score - max_depth, beta + 100);
        for (const auto &move : moves) {
            pos.makemove(move);

            if (pos.is_attacked(pos.get_king(!pos.turn()), pos.turn())) {
                pos.undomove();
                continue;
            }

            const auto prob_cut_score = -search(td, ss + 1, pos, -r_beta, -r_beta + 1, depth - 1 - 3);

            pos.undomove();

            if (prob_cut_score >= r_beta) {
                ss->pv.clear();
                ss->pv.push_back(move);
                return prob_cut_score;
            }
        }
    }

    for (const auto &move : moves) {
        pos.makemove(move);

        if (pos.is_attacked(pos.get_king(!pos.turn()), pos.turn())) {
            pos.undomove();
            continue;
        }

        td.nodes++;
        legal_moves++;

        // Principal Variation Search
        auto score = 0;

        if (legal_moves == 1) {
            score = -search(td, ss + 1, pos, -beta, -alpha, depth - 1);
        } else {
            // LMR
            const auto gives_check = pos.is_attacked(pos.get_kings(pos.turn()), !pos.turn());
            const auto r = reduction(move, depth, legal_moves, in_check, gives_check, is_root);

            score = -search(td, ss + 1, pos, -alpha - 1, -alpha, depth - 1 - r);
            if (score > alpha) {
                score = -search(td, ss + 1, pos, -beta, -alpha, depth - 1);
            }
        }

        pos.undomove();

        if (score > best_score) {
            best_score = score;
            best_move = move;
            ss->pv.clear();
            ss->pv.push_back(move);
        }

        alpha = std::max(alpha, score);
        if (alpha >= beta) {
            if (move.captured() == chess::PieceType::None && depth < 64) {
                td.history_score[chess::index(pos.turn())][chess::index(move.from())][chess::index(move.to())] +=
                    1ULL << depth;
            }
            break;
        }
    }

    if (best_score == std::numeric_limits<int>::min()) {
        if (in_check) {
            return -mate_score + ss->ply;
        } else {
            return 0;
        }
    }

    auto new_ttentry = TTEntry();
    new_ttentry.eval = eval_to_tt(best_score, ss->ply);
    if (best_score <= alpha_orig) {
        new_ttentry.flag = TTFlag::Upper;
    } else if (best_score >= beta) {
        new_ttentry.flag = TTFlag::Lower;
    } else {
        new_ttentry.flag = TTFlag::Exact;
    }
    new_ttentry.depth = depth;
    new_ttentry.move = best_move;
    td.tt->add(pos.hash(), new_ttentry);

    return best_score;
}

}  // namespace swizzles::search

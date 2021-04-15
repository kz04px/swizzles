#include "search.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include "../assert.hpp"
#include "../chess/position.hpp"
#include "alphabeta.hpp"
#include "constants.hpp"
#include "controller.hpp"
#include "pv.hpp"
#include "stack.hpp"

namespace search {

std::pair<chess::Move, chess::Move> search(chess::Position pos) {
    return search(pos,
                  []([[maybe_unused]] int depth,
                     [[maybe_unused]] int score,
                     [[maybe_unused]] std::uint64_t nodes,
                     [[maybe_unused]] int time,
                     [[maybe_unused]] const PV &pv) {});
}

std::pair<chess::Move, chess::Move> search(chess::Position pos, const info_printer_type &info_printer) {
    chess::Move bestmove;
    chess::Move ponder;

    const auto legal_moves = pos.legal_moves();
    if (legal_moves.empty()) {
        return {};
    } else if (legal_moves.size() == 1) {
        return {legal_moves.at(0), {}};
    }

    // Calculate time strategy
    controller.meh();

    // Create search stack
    std::vector<Stack> ss{constants::max_depth + 1};
    for (std::size_t i = 0; i < ss.size(); ++i) {
        ss[i].ply = i;
        ss[i].pv.clear();
        ss[i].killer_1 = chess::Move();
        ss[i].nullmove = true;
    }

    // Iterative deepening
    do {
        int score;

        // Aspiration Windows
        if (controller.depth() < 3) {
            score = alphabeta(pos, &ss[0], -constants::mate_score, constants::mate_score, controller.depth());
        } else {
            constexpr int bounds[] = {50, 200, constants::mate_score};

            for (int b = 0; b < 3; ++b) {
                score = alphabeta(pos, &ss[0], -bounds[b], bounds[b], controller.depth());

                if (-bounds[b] < score && score < bounds[b]) {
                    break;
                }
            }
        }

        if (controller.depth() > 1 && controller.should_stop()) {
            break;
        }

        UCI_ASSERT(!ss[0].pv.empty());
        UCI_ASSERT(legal_pv(pos, ss[0].pv));

        info_printer(controller.depth(), score, controller.nodes(), controller.time_elapsed().count(), ss[0].pv);

        bestmove = ss[0].pv.at(0);
        ponder = ss[0].pv.size() > 1 ? ss[0].pv.at(1) : chess::Move{};
        controller.increment_depth();
    } while (!controller.should_stop());

    UCI_ASSERT(pos.is_legal(bestmove));

    return {bestmove, ponder};
}

}  // namespace search

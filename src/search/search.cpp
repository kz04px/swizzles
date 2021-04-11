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
    return search(pos, [](int depth, int score, std::uint64_t nodes, int time, const PV &pv) {});
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
    std::vector<Stack> ss{max_depth + 1};
    for (std::size_t i = 0; i < ss.size(); ++i) {
        ss.at(i).ply = i;
        ss.at(i).pv.clear();
    }

    // Iterative deepening
    do {
        const auto score = alphabeta(pos, &ss.at(0), -mate_score, mate_score, controller.depth());

        UCI_ASSERT(ss.at(0).pv.size() > 0);
        UCI_ASSERT(legal_pv(pos, ss.at(0).pv));

        if (controller.depth() > 1 && controller.should_stop()) {
            break;
        }

        info_printer(controller.depth(), score, controller.nodes(), controller.time_elapsed().count(), ss.at(0).pv);

        bestmove = ss.at(0).pv.at(0);
        ponder = ss.at(0).pv.size() > 1 ? ss.at(0).pv.at(1) : chess::Move{};
        controller.increment_depth();
    } while (!controller.should_stop());

    UCI_ASSERT(pos.is_legal(bestmove));

    return {bestmove, ponder};
}

}  // namespace search

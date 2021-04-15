#ifndef SORT_HPP
#define SORT_HPP

#include <functional>
#include "chess/move.hpp"

void sort(std::vector<chess::Move> &moves, const std::function<int(const chess::Move &)> &func) {
    int scores[moves.size()];

    // Score
    for (std::size_t i = 0; i < moves.size(); ++i) {
        scores[i] = func(moves[i]);
    }

    // Sort
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
}

#endif

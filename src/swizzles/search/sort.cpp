#include "sort.hpp"
#include <array>
#include "search.hpp"

namespace swizzles::search {

auto sort(chess::MoveList &movelist, const chess::Move ttmove, const ThreadData &td, const chess::Colour turn) noexcept
    -> void {
    if (movelist.empty()) {
        return;
    }

    auto mvvlva = [](const chess::Move move) noexcept -> int {
        const int material[7] = {1, 2, 2, 3, 4, 5, 0};
        return 10 * material[index(move.captured())] - material[index(move.piece())];
    };

    std::array<int, chess::MoveList::max_capacity> scores;

    // Score
    for (std::size_t i = 0; i < movelist.size(); ++i) {
        if (movelist[i] == ttmove) {
            scores[i] = 2'000'000;
        } else if (movelist[i].captured() != chess::PieceType::None) {
            scores[i] = 1'000'000 + mvvlva(movelist[i]);
        } else {
            // history sorting
            auto history_score =
                td.history_score[chess::index(turn)][chess::index(movelist[i].from())][chess::index(movelist[i].to())];
            if (history_score > 700'000) {
                history_score = 700'000;
            }
            scores[i] = history_score;
        }
    }

    // Sort
    for (std::size_t i = 0; i < movelist.size() - 1; ++i) {
        std::size_t best = i;
        for (std::size_t j = i + 1; j < movelist.size(); ++j) {
            if (scores[j] > scores[best]) {
                best = j;
            }
        }

        std::swap(scores[i], scores[best]);
        std::swap(movelist[i], movelist[best]);
    }
}

}  // namespace swizzles::search

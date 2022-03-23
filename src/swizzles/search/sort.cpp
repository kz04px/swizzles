#include "sort.hpp"
#include <array>

namespace swizzles::search {

auto sort(chess::MoveList &movelist) noexcept -> void {
    auto mvvlva = [](const chess::Move move) noexcept -> int {
        const int material[7] = {1, 2, 2, 3, 4, 5, 0};
        return 10 * material[index(move.captured())] - material[index(move.piece())];
    };

    std::array<int, chess::MoveList::max_capacity> scores;

    // Score
    for (std::size_t i = 0; i < movelist.size(); ++i) {
        scores[i] = mvvlva(movelist[i]);
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

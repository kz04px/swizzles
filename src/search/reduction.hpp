#ifndef SEARCH_REDUCTION_HPP
#define SEARCH_REDUCTION_HPP

#include "../chess/move.hpp"

namespace search {

[[nodiscard]] constexpr int reduction(const int move_num,
                                      const int depth,
                                      const bool in_check,
                                      const chess::MoveType move_type) {
    if (move_num < 4 || depth < 3 || in_check || move_type == chess::MoveType::Capture ||
        move_type == chess::MoveType::promo || move_type == chess::MoveType::promo_capture) {
        return 0;
    }
    return 1;
}

}  // namespace search

#endif

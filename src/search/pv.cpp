#include "pv.hpp"

[[nodiscard]] bool legal_pv(chess::Position &pos, const PV &pv) {
    std::size_t depth = 0;

    for (const auto &move : pv) {
        if (!pos.is_legal(move)) {
            break;
        }

        pos.makemove(move);
        depth++;
    }

    for (std::size_t i = 0; i < depth; ++i) {
        pos.undomove();
    }

    return depth == pv.size();
}

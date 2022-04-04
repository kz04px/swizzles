#ifndef SWIZZLES_THREAD_DATA_HPP
#define SWIZZLES_THREAD_DATA_HPP

#include <array>
#include <chess/position.hpp>
#include <cstdint>
#include <memory>
#include <tt.hpp>
#include "../ttentry.hpp"
#include "constants.hpp"
#include "controller.hpp"
#include "stack.hpp"

namespace swizzles::search {

struct ThreadData {
    [[nodiscard]] ThreadData(const int our_id,
                             SearchController *sc,
                             const chess::Position p,
                             std::shared_ptr<TT<TTEntry>> t) noexcept
        : id(our_id), controller(sc), pos(p), tt(t) {
        int i = 0;
        for (auto &s : stack) {
            s.ply = i;
            s.null_move = false;
            i++;
        }

        for (int j = 0; j < 64; j++) {
            for (int k = 0; k < 64; k++) {
                history_score[index(chess::Colour::White)][j][k] = 0;
                history_score[index(chess::Colour::Black)][j][k] = 0;
            }
        }
    }

    int id = 0;
    std::uint64_t nodes = 0;
    int seldepth = 0;
    int tbhits = 0;
    std::array<SearchStack, max_depth + 1> stack;
    int history_score[2][64][64];
    SearchController *controller = nullptr;
    chess::Position pos;
    std::shared_ptr<TT<TTEntry>> tt;
};

}  // namespace swizzles::search

#endif

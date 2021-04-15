#ifndef EVAL_VALUES_HPP
#define EVAL_VALUES_HPP

#include "../score.hpp"

namespace eval::values {

constexpr Score piece_values[] = {
    {100, 100},
    {320, 320},
    {330, 330},
    {500, 500},
    {900, 900},
    {1000000, 1000000},
};
constexpr Score knight_pair = {15, 15};
constexpr Score bishop_pair = {25, 25};
constexpr Score rook_open_file = {25, 25};
constexpr Score turn_bonus = {10, 10};
// Pawns
constexpr Score doubled_pawns = {-10, -10};
constexpr Score isolated_pawn = {-20, -20};
constexpr Score pawn_chain = {10, 10};

}  // namespace eval::values

#endif

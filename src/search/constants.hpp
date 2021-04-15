#ifndef SEARCH_CONSTANTS_HPP
#define SEARCH_CONSTANTS_HPP

namespace constants {

constexpr int mate_score = 30000;
constexpr int max_depth = 128;
constexpr int max_eval = mate_score - max_depth - 1;
constexpr int min_eval = -max_eval;

}  // namespace constants

#endif

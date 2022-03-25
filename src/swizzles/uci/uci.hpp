#ifndef SWIZZLES_UCI_HPP
#define SWIZZLES_UCI_HPP

#include <sstream>
#include "state.hpp"

namespace swizzles::uci {

auto listen() noexcept -> void;
auto parse(std::stringstream &ss, UCIState &state) noexcept -> bool;
auto position(std::stringstream &ss, UCIState &state) noexcept -> void;
auto ucinewgame(UCIState &state) noexcept -> void;
auto moves(std::stringstream &ss, UCIState &state) noexcept -> void;
auto go(std::stringstream &ss, const UCIState &state) noexcept -> void;
auto stop() noexcept -> void;

}  // namespace swizzles::uci

#endif

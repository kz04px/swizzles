#ifndef SWIZZLES_UCI_HPP
#define SWIZZLES_UCI_HPP

#include <chess/position.hpp>
#include <sstream>

namespace swizzles::uci {

auto listen() noexcept -> void;
auto parse(std::stringstream &ss, chess::Position &pos) noexcept -> bool;
auto position(std::stringstream &ss, chess::Position &pos) noexcept -> void;
auto ucinewgame(chess::Position &pos) noexcept -> void;
auto moves(std::stringstream &ss, chess::Position &pos) noexcept -> void;
auto go(std::stringstream &ss, const chess::Position &pos) noexcept -> void;
auto stop() noexcept -> void;

}  // namespace swizzles::uci

#endif

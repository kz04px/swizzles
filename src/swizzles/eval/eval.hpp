#ifndef SWIZZLES_EVAL_HPP
#define SWIZZLES_EVAL_HPP

namespace chess {
class Position;
}  // namespace chess

namespace swizzles::eval {

[[nodiscard]] auto eval(const chess::Position &pos) noexcept -> int;

}  // namespace swizzles::eval

#endif

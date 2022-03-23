#ifndef CHESS_COLOUR_HPP
#define CHESS_COLOUR_HPP

namespace chess {

enum class Colour : int
{
    White = 0,
    Black,
};

[[nodiscard]] constexpr auto operator!(const Colour c) noexcept {
    return static_cast<Colour>(!static_cast<bool>(c));
}

[[nodiscard]] constexpr inline auto index(const Colour c) noexcept -> int {
    return static_cast<int>(c);
}

static_assert(Colour::White == !Colour::Black);
static_assert(Colour::Black == !Colour::White);

}  // namespace chess

#endif

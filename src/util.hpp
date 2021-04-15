#ifndef UTIL_HPP
#define UTIL_HPP

namespace util {

template <typename T>
[[nodiscard]] constexpr T clamp(const T min, const T max, const T val) {
    return (val < min ? min : val > max ? max : val);
}

template <typename T, T min, T max>
[[nodiscard]] constexpr T clamp(const T val) {
    return (val < min ? min : val > max ? max : val);
}

static_assert(clamp(1, 3, 5) == 3);
static_assert(clamp(1, 3, 2) == 2);
static_assert(clamp(1, 3, -2) == 1);
static_assert(clamp(1, 3, 1) == 1);
static_assert(clamp(1, 3, 3) == 3);

static_assert(clamp<int, 1, 3>(0) == 1);
static_assert(clamp<int, 1, 3>(4) == 3);
static_assert(clamp<int, 1, 3>(2) == 2);

}  // namespace util

#endif

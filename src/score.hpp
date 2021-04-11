#ifndef SCORE_HPP
#define SCORE_HPP

class Score {
   public:
    using value_type = int;

    constexpr Score() : mg_{0}, eg_{0} {
    }

    constexpr Score(const value_type mg, const value_type eg) : mg_{mg}, eg_{eg} {
    }

    [[nodiscard]] constexpr Score operator+(const Score &rhs) const noexcept {
        return {mg_ + rhs.mg_, eg_ + rhs.eg_};
    }

    [[nodiscard]] constexpr Score operator-(const Score &rhs) const noexcept {
        return {mg_ - rhs.mg_, eg_ - rhs.eg_};
    }

    [[nodiscard]] constexpr Score operator-() const noexcept {
        return {-mg_, -eg_};
    }

    [[nodiscard]] constexpr Score operator*(const value_type n) const noexcept {
        return {mg_ * n, eg_ * n};
    }

    constexpr Score operator+=(const Score &rhs) noexcept {
        mg_ += rhs.mg_;
        eg_ += rhs.eg_;
        return *this;
    }

    constexpr Score operator-=(const Score &rhs) noexcept {
        mg_ -= rhs.mg_;
        eg_ -= rhs.eg_;
        return *this;
    }

    [[nodiscard]] constexpr bool operator==(const Score &rhs) const noexcept {
        return mg_ == rhs.mg_ && eg_ == rhs.eg_;
    }

    [[nodiscard]] constexpr value_type mg() const noexcept {
        return mg_;
    }

    [[nodiscard]] constexpr value_type eg() const noexcept {
        return eg_;
    }

   private:
    value_type mg_;
    value_type eg_;
};

static_assert(Score{1, 2} + Score{3, 4} == Score{4, 6});
static_assert(Score{1, 2} * 2 == Score{2, 4});
static_assert(-Score{1, 2} == Score{-1, -2});

#endif

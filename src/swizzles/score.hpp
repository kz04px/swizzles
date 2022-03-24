#ifndef SWIZZLES_SCORE_HPP
#define SWIZZLES_SCORE_HPP

class Score {
   public:
    using value_type = int;

    [[nodiscard]] constexpr Score() = default;

    [[nodiscard]] constexpr Score(const value_type mg, const value_type eg) : m_mg{mg}, m_eg{eg} {
    }

    [[nodiscard]] constexpr auto operator+(const Score &rhs) const noexcept -> Score {
        return {m_mg + rhs.m_mg, m_eg + rhs.m_eg};
    }

    [[nodiscard]] constexpr auto operator-(const Score &rhs) const noexcept -> Score {
        return {m_mg - rhs.m_mg, m_eg - rhs.m_eg};
    }

    [[nodiscard]] constexpr auto operator-() const noexcept -> Score {
        return {-m_mg, -m_eg};
    }

    [[nodiscard]] constexpr auto operator*(const value_type n) const noexcept -> Score {
        return {m_mg * n, m_eg * n};
    }

    constexpr auto operator+=(const Score &rhs) noexcept -> Score {
        m_mg += rhs.m_mg;
        m_eg += rhs.m_eg;
        return *this;
    }

    constexpr auto operator-=(const Score &rhs) noexcept -> Score {
        m_mg -= rhs.m_mg;
        m_eg -= rhs.m_eg;
        return *this;
    }

    [[nodiscard]] constexpr auto operator==(const Score &rhs) const noexcept -> bool {
        return m_mg == rhs.m_mg && m_eg == rhs.m_eg;
    }

    [[nodiscard]] constexpr auto mg() const noexcept -> value_type {
        return m_mg;
    }

    [[nodiscard]] constexpr auto eg() const noexcept -> value_type {
        return m_eg;
    }

   private:
    value_type m_mg = 0;
    value_type m_eg = 0;
};

static_assert(Score{1, 2} + Score{3, 4} == Score{4, 6});
static_assert(Score{1, 2} * 2 == Score{2, 4});
static_assert(-Score{1, 2} == Score{-1, -2});

#endif

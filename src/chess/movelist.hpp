#ifndef CHESS_MOVELIST_HPP
#define CHESS_MOVELIST_HPP

#include <array>
#include "move.hpp"

namespace chess {

class MoveList {
   public:
    static constexpr std::size_t max_capacity = 218;

    using size_type = std::size_t;
    using iterator = std::array<Move, max_capacity>::iterator;
    using const_iterator = std::array<Move, max_capacity>::const_iterator;

    [[nodiscard]] constexpr MoveList() noexcept = default;

    [[nodiscard]] constexpr auto capacity() const noexcept -> size_type {
        return max_capacity;
    }

    [[nodiscard]] constexpr auto size() const noexcept -> size_type {
        return m_size;
    }

    [[nodiscard]] constexpr auto empty() const noexcept -> bool {
        return m_size == 0;
    }

    [[nodiscard]] constexpr auto begin() noexcept -> iterator {
        return m_moves.begin();
    }

    [[nodiscard]] constexpr auto end() noexcept -> iterator {
        return std::next(m_moves.begin(), m_size);
    }

    [[nodiscard]] constexpr auto begin() const noexcept -> const_iterator {
        return m_moves.begin();
    }

    [[nodiscard]] constexpr auto end() const noexcept -> const_iterator {
        return std::next(m_moves.begin(), m_size);
    }

    [[nodiscard]] constexpr auto operator[](const std::size_t idx) const noexcept -> const Move& {
        return m_moves[idx];
    }

    [[nodiscard]] constexpr auto operator[](const std::size_t idx) noexcept -> Move& {
        return m_moves[idx];
    }

    auto clear() noexcept -> void {
        m_size = 0;
    }

    template <typename... Args>
    auto emplace_back(Args... args) noexcept -> void {
        m_moves[m_size] = std::move(Move(args...));
        m_size++;
    }

   private:
    std::array<Move, max_capacity> m_moves;
    size_type m_size = 0;
};

}  // namespace chess

#endif

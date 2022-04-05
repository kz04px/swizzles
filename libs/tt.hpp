#ifndef TT_HPP
#define TT_HPP

#include <cstdint>
#include <cstring>
#include <memory>

template <class T>
class TT {
   public:
    [[nodiscard]] constexpr TT() = default;

    [[nodiscard]] explicit TT(unsigned int mb) {
        if (mb == 0) {
            mb = 1;
        }
        m_max_entries = (mb * 1024 * 1024) / sizeof(T);
        m_entries = std::make_unique<T[]>(m_max_entries);
    }

    [[nodiscard]] auto poll(const std::uint64_t hash) const noexcept -> T {
        const auto idx = index(hash);
        return m_entries[idx];
    }

    [[nodiscard]] auto size() const noexcept -> std::size_t {
        return m_max_entries;
    }

    [[nodiscard]] auto hashfull() const noexcept -> int {
        const auto size = std::min(m_max_entries, static_cast<std::size_t>(100));
        std::size_t filled = 0;
        for (std::size_t i = 0; i < size; ++i) {
            filled += (m_entries[i] != 0);
        }
        return 1000 * (static_cast<double>(filled) / size);
    }

    auto add(const std::uint64_t hash, const T &t) noexcept -> void {
        const auto idx = index(hash);
        m_entries[idx] = t;
    }

    auto clear() noexcept -> void {
        for (std::size_t i = 0; i < m_max_entries; ++i) {
            m_entries[i] = T();
        }
    }

    auto prefetch(const std::uint64_t hash) const noexcept -> void {
        const auto idx = index(hash);
        __builtin_prefetch(&m_entries[idx]);
    }

   private:
    [[nodiscard]] auto index(const std::uint64_t hash) const noexcept -> std::size_t {
        return hash % m_max_entries;
    }

    std::size_t m_max_entries = 0;
    std::unique_ptr<T[]> m_entries;
};

#endif

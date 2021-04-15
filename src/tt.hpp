#ifndef TT_HPP
#define TT_HPP

#include <cstdint>
#include <vector>

template <class entry_type, class key_type = std::uint64_t>
class TT {
   public:
    TT() : m_entries{} {
    }

    TT(unsigned int mb) : m_entries{} {
        resize(mb);
    }

    [[nodiscard]] entry_type poll(const key_type hash) const noexcept {
        const auto idx = index(hash);
        return m_entries[idx];
    }

    [[nodiscard]] int hashfull() const noexcept {
        const auto n = std::min(100, m_entries.size());
        int used = 0;

        for (int i = 0; i < n; ++i) {
            used += m_entries[i] != key_type();
        }

        return 1000 * used / n;
    }

    void resize(const unsigned int mb) {
        const std::size_t num_entries = (1024 * 1024 * mb) / sizeof(entry_type);
        m_entries.resize(num_entries);
    }

    void add(const key_type hash, const entry_type &t) noexcept {
        const auto idx = index(hash);
        m_entries[idx] = t;
    }

    void clear() noexcept {
        for (auto &entry : m_entries) {
            entry = entry_type();
        }
    }

    void prefetch(const key_type hash) const noexcept {
        const auto idx = index(hash);
        __builtin_prefetch(&m_entries[idx]);
    }

   private:
    [[nodiscard]] std::size_t index(const key_type hash) const noexcept {
        return hash % m_entries.size();
    }

    std::vector<entry_type> m_entries;
};

#endif

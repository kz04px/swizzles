#ifndef SWIZZLES_SEARCH_CONTROLLER_HPP
#define SWIZZLES_SEARCH_CONTROLLER_HPP

#include <atomic>
#include <chrono>
#include <cstdint>
#include "settings.hpp"

namespace swizzles::search {

class SearchController {
   public:
    using clock_type = std::chrono::steady_clock;

    [[nodiscard]] SearchController(const SearchSettings &settings, std::atomic<bool> &stop) noexcept
        : m_settings(settings), m_stop(stop) {
    }

    [[nodiscard]] auto should_stop() const noexcept -> bool {
        return m_stop;
    }

    [[nodiscard]] auto elapsed() const noexcept -> std::chrono::milliseconds {
        const auto t1 = clock_type::now();
        const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - m_start_time);
        return dt;
    }

    auto stop() noexcept -> void {
        m_stop = true;
    }

    auto resume() noexcept -> void {
        m_stop = false;
    }

    auto set_depth(const int n) noexcept -> void {
        m_current_depth = n;
    }

    auto set_max_time(const int n) noexcept -> void {
        m_target_time = n;
    }

    auto start() noexcept -> void {
        m_start_time = clock_type::now();
    }

    auto update() noexcept -> void {
        switch (m_settings.type) {
            case SearchType::Time:
                m_stop = m_stop || elapsed().count() >= m_target_time;
                break;
            case SearchType::Depth:
                m_stop = m_stop || m_current_depth > m_settings.depth;
                break;
            case SearchType::Movetime:
                m_stop = m_stop || elapsed().count() >= m_settings.movetime;
                break;
            case SearchType::Nodes:
                break;
            case SearchType::Infinite:
                break;
        }
    }

   private:
    SearchSettings m_settings;
    clock_type::time_point m_start_time;
    int m_current_depth = 0;
    int m_target_time = 0;
    std::atomic<bool> &m_stop;
};

}  // namespace swizzles::search

#endif

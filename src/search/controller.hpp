#ifndef SEARCH_CONTROLLER_HPP
#define SEARCH_CONTROLLER_HPP

#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include "../chess/side.hpp"

class Controller {
   public:
    enum SearchType : int
    {
        Time = 0,
        Depth,
        Movetime,
        Nodes,
        Infinite
    };

    using ms = std::chrono::milliseconds;
    using time_point = std::chrono::_V2::system_clock::time_point;

    Controller()
        : nodes_{},
          depth_{},
          search_type_{SearchType::Depth},
          search_time_{},
          max_nodes_{},
          max_depth_{1},
          movestogo_{},
          us_{},
          movetime_{},
          wtime_{},
          winc_{},
          btime_{},
          binc_{},
          start_{},
          stop_{} {
    }

    void set_depth(int depth) noexcept {
        search_type_ = SearchType::Depth;
        max_depth_ = depth;
    }

    void set_movetime(int movetime) noexcept {
        search_type_ = SearchType::Movetime;
        movetime_ = static_cast<ms>(movetime);
    }

    void set_nodes(std::uint64_t nodes) noexcept {
        search_type_ = SearchType::Nodes;
        max_nodes_ = nodes;
    }

    void set_wtime(int time) noexcept {
        search_type_ = SearchType::Time;
        wtime_ = static_cast<ms>(time);
    }

    void set_btime(int time) noexcept {
        search_type_ = SearchType::Time;
        btime_ = static_cast<ms>(time);
    }

    void set_winc(int inc) noexcept {
        search_type_ = SearchType::Time;
        winc_ = static_cast<ms>(inc);
    }

    void set_binc(int inc) noexcept {
        search_type_ = SearchType::Time;
        binc_ = static_cast<ms>(inc);
    }

    void set_infinite() noexcept {
        search_type_ = SearchType::Infinite;
    }

    void set_movestogo(int movestogo) noexcept {
        movestogo_ = movestogo;
    }

    void set_us(chess::Side us) noexcept {
        us_ = us;
    }

    void reset() noexcept {
        movetime_ = std::chrono::milliseconds(0);
        wtime_ = std::chrono::milliseconds(0);
        winc_ = std::chrono::milliseconds(0);
        btime_ = std::chrono::milliseconds(0);
        binc_ = std::chrono::milliseconds(0);
        stop_ = false;
        nodes_ = 0;
        depth_ = 1;
        start_ = std::chrono::high_resolution_clock::now();
    }

    void increment_nodes() noexcept {
        nodes_++;
    }

    void increment_depth() noexcept {
        depth_++;
    }

    [[nodiscard]] ms time_elapsed() const noexcept {
        return std::chrono::duration_cast<ms>(now() - start_);
    }

    [[nodiscard]] auto nodes() const noexcept {
        return nodes_;
    }

    [[nodiscard]] auto depth() const noexcept {
        return depth_;
    }

    void stop() noexcept {
        stop_ = true;
    }

    void meh() noexcept {
        if (search_type_ != SearchType::Time) {
            return;
        }

        const auto time_us = us_ == chess::Side::White ? wtime_ : btime_;
        const auto time_them = us_ == chess::Side::White ? btime_ : wtime_;
        const auto moves_left = movestogo_ > 0 ? movestogo_ : 30;

        search_time_ = time_us / moves_left;
    }

    [[nodiscard]] bool should_stop() const noexcept {
        if (stop_ || depth_ > 128) {
            return true;
        }

        switch (search_type_) {
            case SearchType::Time:
                return now() >= start_ + search_time_;
            case SearchType::Depth:
                return depth_ > max_depth_;
            case SearchType::Movetime:
                return now() >= start_ + movetime_;
            case SearchType::Nodes:
                return nodes_ >= max_nodes_;
            case SearchType::Infinite:
                return false;
            default:
                std::exit(1);
        }

        return false;
    }

   private:
    [[nodiscard]] time_point now() const noexcept {
        return std::chrono::high_resolution_clock::now();
    }

    // Statistics
    std::uint64_t nodes_;
    int depth_;
    // Settings
    SearchType search_type_;
    ms search_time_;
    std::uint64_t max_nodes_;
    int max_depth_;
    int movestogo_;
    chess::Side us_;
    ms movetime_;
    ms wtime_;
    ms winc_;
    ms btime_;
    ms binc_;
    time_point start_;
    bool stop_;
};

extern Controller controller;

#endif

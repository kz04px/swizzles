#include <chess/position.hpp>
#include <chrono>
#include <thread>
#include "controller.hpp"
#include "root.hpp"
#include "search.hpp"

namespace swizzles::search {

[[nodiscard]] auto get_nodes(const std::vector<ThreadData> &thread_data) noexcept -> std::uint64_t {
    std::uint64_t total = 0;
    for (const auto &data : thread_data) {
        total += data.nodes;
    }
    return total;
}

[[nodiscard]] auto get_seldepth(const std::vector<ThreadData> &thread_data) noexcept -> int {
    int seldepth = 0;
    for (const auto &data : thread_data) {
        seldepth = std::max(data.seldepth, seldepth);
    }
    return seldepth;
}

[[nodiscard]] auto root(const uci::UCIState &state, const SearchSettings settings, std::atomic<bool> &stop) noexcept
    -> Results {
    auto controller = SearchController(settings, stop);
    controller.start();

    // Time strategy
    if (settings.type == SearchType::Time) {
        const auto time = state.pos.turn() == chess::Colour::White ? settings.wtime : settings.btime;
        controller.set_max_time(time / 30);
    }

    auto results = Results();

    std::vector<ThreadData> thread_data;
    for (int i = 0; i < state.threads.val; ++i) {
        thread_data.emplace_back(i, &controller, state.pos, state.tt);
    }

    std::vector<std::thread> threads;

    for (int depth = 1; depth < max_depth; ++depth) {
        controller.set_depth(depth);

        // Start the main search
        const auto eval = swizzles::search::search(
            thread_data[0], &thread_data[0].stack[0], thread_data[0].pos, -inf_score, inf_score, depth);

        // Remember if the search controller stopped the search
        const auto controller_stoppage = controller.should_stop();

        // Tell helpers to stop
        controller.stop();

        // Wait for helpers
        for (auto &thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        controller.resume();

        if (controller_stoppage) {
            break;
        }

        // Gather statistics
        const auto dt = controller.elapsed();
        const auto bestmove = thread_data[0].stack[0].pv.size() > 0 ? thread_data[0].stack[0].pv[0] : chess::Move();
        const auto ponder = thread_data[0].stack[0].pv.size() > 1 ? thread_data[0].stack[0].pv[1] : chess::Move();
        const auto nodes = get_nodes(thread_data);
        const auto seldepth = get_seldepth(thread_data);
        const auto nps = dt.count() == 0 ? 0 : (1000 * nodes) / dt.count();
        const auto hashfull = 0;
        const auto tbhits = 0;

        // Update search results
        results = Results(bestmove, ponder, nodes, depth, seldepth, eval, dt.count(), tbhits);

        // Print
        settings.info_printer(depth,
                              results.seldepth,
                              eval,
                              dt.count(),
                              results.nodes,
                              nps,
                              results.tbhits,
                              hashfull,
                              thread_data[0].stack[0].pv);
    }

    return results;
}

}  // namespace swizzles::search

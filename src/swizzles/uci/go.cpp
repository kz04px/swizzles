#include <cstdint>
#include <iostream>
#include <thread>
#include "../search/pv.hpp"
#include "../search/root.hpp"
#include "../search/settings.hpp"
#include "../settings.hpp"
#include "uci.hpp"

namespace swizzles::uci {

std::thread search_thread;
std::atomic<bool> search_stop = false;
auto uci_info_printer = [](const int depth,
                           const int seldepth,
                           const int eval,
                           const std::uint64_t ms,
                           const std::uint64_t nodes,
                           const std::uint64_t nps,
                           const int tbhits,
                           const int hashfull,
                           const search::PV &pv) {
    std::cout << "info";
    std::cout << " depth " << depth;
    std::cout << " seldepth " << seldepth;
    std::cout << " score cp " << eval;
    std::cout << " time " << ms;
    std::cout << " nodes " << nodes;
    std::cout << " nps " << nps;
    std::cout << " tbhits " << tbhits;
    std::cout << " hashfull " << hashfull;
    if (!pv.empty()) {
        std::cout << " pv";
        for (const auto move : pv) {
            std::cout << " " << move;
        }
    }
    std::cout << std::endl;
};

auto stop() noexcept -> void {
    search_stop = true;
    if (search_thread.joinable()) {
        search_thread.join();
    }
    search_stop = false;
}

auto go(std::stringstream &ss, const UCIState &state) noexcept -> void {
    stop();

    // Search settings
    auto settings = search::SearchSettings();
    settings.info_printer = uci_info_printer;

    while (!ss.eof()) {
        std::string word;
        ss >> word;
        if (word == "wtime") {
            ss >> word;
            settings.type = search::SearchType::Time;
            settings.wtime = std::stoi(word);
        } else if (word == "btime") {
            ss >> word;
            settings.type = search::SearchType::Time;
            settings.btime = std::stoi(word);
        } else if (word == "winc") {
            ss >> word;
            settings.type = search::SearchType::Time;
            settings.binc = std::stoi(word);
        } else if (word == "winc") {
            ss >> word;
            settings.type = search::SearchType::Time;
            settings.binc = std::stoi(word);
        } else if (word == "depth") {
            ss >> word;
            settings.type = search::SearchType::Depth;
            settings.depth = std::stoi(word);
        } else if (word == "movetime") {
            ss >> word;
            settings.type = search::SearchType::Movetime;
            settings.movetime = std::stoi(word);
        } else if (word == "nodes") {
            // ss >> word;
            // settings.type = search::SearchType::Nodes;
            // settings.nodes = std::stoi(word);
        } else if (word == "movestogo") {
            ss >> word;
            settings.movestogo = std::stoi(word);
        } else if (word == "infinite") {
            settings.type = search::SearchType::Infinite;
        }
    }

    search_thread = std::thread([state, settings]() {
        const auto results = search::root(state, settings, search_stop);
        std::cout << "bestmove " << results.bestmove << std::endl;
    });
}

}  // namespace swizzles::uci

#ifndef SWIZZLES_UCI_STATE_HPP
#define SWIZZLES_UCI_STATE_HPP

#include <chess/position.hpp>
#include <memory>
#include <tt.hpp>
#include "../settings.hpp"
#include "../ttentry.hpp"

namespace swizzles::uci {

struct UCIState {
    chess::Position pos = chess::Position("startpos");
    std::shared_ptr<TT<TTEntry>> tt;
    settings::Spin hash = settings::Spin("Hash", 1, 128, 1);
    settings::Spin threads = settings::Spin("Threads", 1, 4, 1);
};

}  // namespace swizzles::uci

#endif

#ifndef SWIZZLES_UCI_STATE_HPP
#define SWIZZLES_UCI_STATE_HPP

#include <chess/position.hpp>
#include "../settings.hpp"

namespace swizzles::uci {

struct UCIState {
    chess::Position pos = chess::Position("startpos");
    settings::Spin hash = settings::Spin("Hash", 1, 128, 1);
    settings::Spin threads = settings::Spin("Threads", 1, 4, 1);
};

}  // namespace swizzles::uci

#endif

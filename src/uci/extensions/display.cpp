#include <iostream>
#include "../../chess/position.hpp"
#include "extensions.hpp"

namespace uci::extension {

void display(const chess::Position &pos) {
    std::cout << pos << std::endl;
    std::cout << pos.get_fen() << std::endl;
}

}  // namespace uci::extension

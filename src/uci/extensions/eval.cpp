#include "../../eval/eval.hpp"
#include <iostream>
#include "../uci.hpp"
#include "extensions.hpp"

namespace uci::extension {

void eval(const chess::Position &pos) {
    const auto score = eval::eval(pos);
    std::cout << "info string eval " << score << std::endl;
}

}  // namespace uci::extension

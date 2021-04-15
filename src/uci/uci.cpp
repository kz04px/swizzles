#include "uci.hpp"
#include <iostream>
#include "../globals.hpp"

namespace uci {

void uci() {
    std::cout << "id name Swizzles\n";
    std::cout << "id author kz04px\n";

    std::cout << settings::hash << "\n";

    std::cout << "uciok" << std::endl;
}

}  // namespace uci

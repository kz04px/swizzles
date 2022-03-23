#include <iostream>
#include "uci/uci.hpp"

int main() {
    std::string input;
    std::getline(std::cin, input);

    if (input == "uci") {
        swizzles::uci::listen();
    } else if (input == "about") {
#ifdef NDEBUG
        std::cout << "Build: Release\n";
#else
        std::cout << "Build: Debug\n";
#endif
        std::cout << "Date: " << __DATE__ << "\n";
        std::cout << "Time: " << __TIME__ << "\n";
        std::cout << "URL: https://github.com/kz04px/swizzles\n";
    }

    return 0;
}

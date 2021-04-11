#include <iostream>
#include "uci/uci.hpp"

int main() {
    std::setbuf(stdin, NULL);
    std::setbuf(stdout, NULL);

    srand(time(0));

#ifndef NDEBUG
    std::cout << "DEBUG BUILD" << std::endl;
#endif

    std::string input;
    std::getline(std::cin, input);

    if (input == "uci") {
        uci::listen();
    } else if (input == "about") {
        std::cout << "Swizzles\n";
        std::cout << "Authored by kz04px\n";
        std::cout << "C++ Chess engine\n";
        std::cout << "Date " << __DATE__ << "\n";
        std::cout << "Time " << __TIME__ << "\n";
    } else {
        std::cerr << "Protocol unsupported\n";
    }

    return 0;
}

#include "../globals.hpp"
#include "../util.hpp"
#include "uci.hpp"

namespace uci {

void setoption(std::stringstream &ss) {
    std::string word;

    ss >> word;
    if (word != "name") {
        return;
    }

    // Collect option name
    std::string name = "";
    while (ss >> word && word != "value") {
        if (name != "") {
            name += " ";
        }
        name += word;
    }

    // Collect option value
    std::string value = "";
    while (ss >> word) {
        if (value != "") {
            value += " ";
        }
        value += word;
    }

    try {
        if (name == "Hash") {
            settings::hash.set(std::stoi(value));
        }
    } catch (...) {
    }
}

}  // namespace uci

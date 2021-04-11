#ifndef UCI_EXTENSIONS_HPP
#define UCI_EXTENSIONS_HPP

#include <sstream>

namespace chess {

class Position;

}  // namespace chess

namespace uci::extension {

void perft(chess::Position &pos, std::stringstream &ss);
void split(chess::Position &pos, std::stringstream &ss);
void display(const chess::Position &pos);
void eval(const chess::Position &pos);

}  // namespace uci::extension

#endif

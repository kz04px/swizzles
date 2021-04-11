#ifndef UCI_HPP
#define UCI_HPP

#include <functional>
#include <sstream>

namespace chess {

class Position;

}  // namespace chess

namespace uci {

void listen();

void uci();
void isready();
void ucinewgame(chess::Position &pos);
void position(chess::Position &pos, std::stringstream &ss);
void moves(chess::Position &pos, std::stringstream &ss);
void go(chess::Position &pos, std::stringstream &ss);
void setoption(std::stringstream &ss);
void stop();

}  // namespace uci

#endif

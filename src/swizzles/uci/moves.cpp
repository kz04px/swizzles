#include "uci.hpp"

namespace swizzles::uci {

auto moves(std::stringstream &ss, chess::Position &pos) noexcept -> void {
    while (!ss.eof()) {
        std::string word;
        ss >> word;

        // Handle castling
        // - If we get send a castling move in the form 'e1g1' we can't string match it with 'e1h1'
        const auto wksc = word == "e1g1" && pos.get_king(chess::Colour::White) == chess::Square::E1;
        const auto wqsc = word == "e1c1" && pos.get_king(chess::Colour::White) == chess::Square::E1;
        const auto bksc = word == "e8g8" && pos.get_king(chess::Colour::Black) == chess::Square::E8;
        const auto bqsc = word == "e8c8" && pos.get_king(chess::Colour::Black) == chess::Square::E8;
        const auto ksc = wksc | bksc;
        const auto qsc = wqsc | bqsc;

        bool found = false;
        const auto moves = pos.movegen();
        for (const auto &move : moves) {
            if ((ksc && move.type() == chess::MoveType::KSC) || (qsc && move.type() == chess::MoveType::QSC) ||
                static_cast<std::string>(move) == word) {
                pos.makemove(move);
                found = true;
                break;
            }
        }

        if (!found) {
            break;
        }
    }
}

}  // namespace swizzles::uci

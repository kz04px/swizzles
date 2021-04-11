#ifndef LIBCHESS_VALIDATE_HPP
#define LIBCHESS_VALIDATE_HPP

#include <string>

namespace chess::validate {

[[nodiscard]] bool fen(const std::string &fen) {
    return true;
}

}  // namespace chess::validate

#endif

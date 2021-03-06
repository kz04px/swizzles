#include "position.hpp"

namespace chess {

[[nodiscard]] std::size_t Position::count_moves() const noexcept {
    return legal_moves().size();
}

}  // namespace chess

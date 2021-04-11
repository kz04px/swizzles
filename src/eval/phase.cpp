#include "phase.hpp"

namespace eval {

[[nodiscard]] int phase(const chess::Position &pos, const Score &score) noexcept {
    const int num_knights = pos.occupancy(chess::Piece::Knight).count();
    const int num_bishops = pos.occupancy(chess::Piece::Bishop).count();
    const int num_rooks = pos.occupancy(chess::Piece::Rook).count();
    const int num_queens = pos.occupancy(chess::Piece::Queen).count();
    const int total_phase = 24;
    int phase = total_phase - num_knights - num_bishops - 2 * num_rooks - 4 * num_queens;
    phase = (phase * 256 + (total_phase / 2)) / total_phase;
    return ((score.mg() * (256 - phase)) + (score.eg() * phase)) / 256;
}

}  // namespace eval

#include "eval.hpp"
#include <chess/magic.hpp>
#include <chess/position.hpp>
#include "pst.hpp"

namespace swizzles::eval {

static constexpr std::array<Score, 6> material = {{
    {100, 100},
    {300, 300},
    {300, 300},
    {500, 500},
    {900, 900},
    {0, 0},
}};

static int bishopMgMobility[14] = {-25, -11, -6, -1, 3, 6, 9, 12, 14, 17, 19, 21, 23, 25};

static int bishopEgMobility[14] = {-50, -22, -11, -2, 6, 12, 18, 24, 29, 34, 38, 42, 46, 50};

static int rookMgMobility[15] = {-10, -4, -2, 0, 2, 3, 4, 5, 6, 8, 8, 9, 10, 11, 12};

static int rookEgMobility[15] = {-50, -22, -11, -2, 6, 12, 18, 24, 29, 34, 38, 42, 46, 50, 54};

static int queenMgMobility[28] = {-10, -6, -5, -4, -2, -2, -1, 0,  1, 2, 2, 3, 3, 4, 4, 5, 6, 6, 6,

                                  7,   7,  8,  8,  9,  9,  10, 10, 10};

static int queenEgMobility[28] = {-50, -30, -22, -16, -10, -6, -2, 2,  6,  10, 13, 16, 19, 22, 24,

                                  27,  30,  32,  34,  37,  39, 41, 43, 45, 47, 50, 51, 53};

template <chess::Colour us>
[[nodiscard]] auto eval_us(const chess::Position &pos) noexcept -> Score {
    Score score;

    // Material
    score += material[0] * pos.get_pawns(us).count();
    score += material[1] * pos.get_knights(us).count();
    score += material[2] * pos.get_bishops(us).count();
    score += material[3] * pos.get_rooks(us).count();
    score += material[4] * pos.get_queens(us).count();

    // PST
    for (const auto sq : pos.get_pawns(us)) {
        score += pst_value<us>(chess::PieceType::Pawn, sq);
    }
    for (const auto sq : pos.get_knights(us)) {
        score += pst_value<us>(chess::PieceType::Knight, sq);
    }
    for (const auto sq : pos.get_bishops(us)) {
        score += pst_value<us>(chess::PieceType::Bishop, sq);
    }
    for (const auto sq : pos.get_rooks(us)) {
        score += pst_value<us>(chess::PieceType::Rook, sq);
    }
    for (const auto sq : pos.get_queens(us)) {
        score += pst_value<us>(chess::PieceType::Queen, sq);
    }
    for (const auto sq : pos.get_kings(us)) {
        score += pst_value<us>(chess::PieceType::King, sq);
    }

    // Mobility for bishops, rooks, and queens
    for (const auto square : pos.get_bishops(us)) {
        auto count = chess::magic::bishop_moves(square, pos.get_occupied()).count();
        score += Score{bishopMgMobility[count], bishopEgMobility[count]};
    }
    for (const auto square : pos.get_rooks(us)) {
        auto count = chess::magic::rook_moves(square, pos.get_occupied()).count();
        score += Score{rookMgMobility[count], rookEgMobility[count]};
    }
    for (const auto square : pos.get_queens(us)) {
        auto count = chess::magic::queen_moves(square, pos.get_occupied()).count();
        score += Score{queenMgMobility[count], queenEgMobility[count]};
    }

    return score;
}

[[nodiscard]] auto phase(const chess::Position &pos, const Score &score) noexcept -> int {
    const auto num_knights = pos.get_knights().count();
    const auto num_bishops = pos.get_bishops().count();
    const auto num_rooks = pos.get_rooks().count();
    const auto num_queens = pos.get_queens().count();
    const auto total_phase = 24;
    auto phase = total_phase - num_knights - num_bishops - 2 * num_rooks - 4 * num_queens;
    phase = (phase * 256 + (total_phase / 2)) / total_phase;
    return ((score.mg() * (256 - phase)) + (score.eg() * phase)) / 256;
}

[[nodiscard]] auto eval(const chess::Position &pos) noexcept -> int {
    Score score;
    score += eval_us<chess::Colour::White>(pos);
    score -= eval_us<chess::Colour::Black>(pos);
    const int phased = phase(pos, score);

    if (pos.turn() == chess::Colour::White) {
        return phased;
    } else {
        return -phased;
    }
}

}  // namespace swizzles::eval

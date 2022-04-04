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

static constexpr std::array<Score, 14> bishop_mob_bonus = {{
    {-25, -50},
    {-11, -22},
    {-6, -11},
    {-1, -2},
    {3, 6},
    {6, 12},
    {9, 18},
    {12, 24},
    {14, 29},
    {17, 34},
    {19, 38},
    {21, 42},
    {23, 46},
    {25, 50},
}};

static constexpr std::array<Score, 15> rook_mob_bonus = {{
    {-10, -50},
    {-4, -22},
    {-2, -11},
    {0, -2},
    {2, 6},
    {3, 12},
    {4, 18},
    {5, 24},
    {6, 29},
    {8, 34},
    {8, 38},
    {9, 42},
    {10, 46},
    {11, 50},
    {12, 54},
}};

static constexpr std::array<Score, 28> queen_mob_bonus = {{
    {-10, -50}, {-6, -30}, {-5, -22}, {-4, -16}, {-2, -10}, {-2, -6}, {-1, -2}, {0, 2},   {1, 6},  {2, 10},
    {2, 13},    {3, 16},   {3, 19},   {4, 22},   {4, 24},   {5, 27},  {6, 30},  {6, 32},  {6, 34}, {7, 37},
    {7, 39},    {8, 41},   {8, 43},   {9, 45},   {9, 47},   {10, 50}, {10, 51}, {10, 53},
}};

template <chess::Colour us>
[[nodiscard]] constexpr auto shield(const chess::Square sq) noexcept -> chess::Bitboard {
    auto bb = chess::Bitboard(sq);
    if constexpr (us == chess::Colour::White) {
        bb = bb.north();
        bb |= bb.north();
    } else {
        bb = bb.south();
        bb |= bb.south();
    }
    bb |= bb.east();
    bb |= bb.west();
    return bb;
}

static_assert(shield<chess::Colour::White>(chess::Square::G1) == chess::Bitboard(0xE0E000ULL));
static_assert(shield<chess::Colour::Black>(chess::Square::G8) == chess::Bitboard(0xE0E00000000000ULL));
static_assert(shield<chess::Colour::White>(chess::Square::A1) == chess::Bitboard(0x30300ULL));
static_assert(shield<chess::Colour::Black>(chess::Square::A8) == chess::Bitboard(0x3030000000000ULL));

template <chess::Colour us>
[[nodiscard]] auto king_safety(const chess::Position &pos) noexcept -> Score {
    const auto ksq = pos.get_king(us);
    const auto pawns = shield<us>(ksq) & pos.get_pawns(us);
    return Score{20, 0} * pawns.count();
}

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
        const auto count = chess::magic::bishop_moves(square, pos.get_occupied()).count();
        score += bishop_mob_bonus[count];
    }
    for (const auto square : pos.get_rooks(us)) {
        const auto count = chess::magic::rook_moves(square, pos.get_occupied()).count();
        score += rook_mob_bonus[count];
    }
    for (const auto square : pos.get_queens(us)) {
        const auto count = chess::magic::queen_moves(square, pos.get_occupied()).count();
        score += queen_mob_bonus[count];
    }

    // King safety
    score += king_safety<us>(pos);

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

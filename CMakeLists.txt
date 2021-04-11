cmake_minimum_required(VERSION 3.10)

# Project
project(Swizzles VERSION 1.0 LANGUAGES CXX)

# Find packages
find_package(Threads REQUIRED)

# Flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add engine executable
add_executable(
    swizzles

    src/main.cpp

    # UCI
    src/uci/isready.cpp
    src/uci/listen.cpp
    src/uci/moves.cpp
    src/uci/position.cpp
    src/uci/search.cpp
    src/uci/setoption.cpp
    src/uci/uci.cpp
    src/uci/ucinewgame.cpp

    # UCI - Extensions
    src/uci/extensions/display.cpp
    src/uci/extensions/eval.cpp
    src/uci/extensions/perft.cpp
    src/uci/extensions/split.cpp

    # Search
    src/search/pv.cpp
    src/search/controller.cpp
    src/search/alphabeta.cpp
    src/search/search.cpp

    # Eval
    src/eval/eval.cpp
    src/eval/phase.cpp

    # Chess
    src/chess/attackers.cpp
    src/chess/is_legal.cpp
    src/chess/predict_hash.cpp
    src/chess/checkers.cpp
    src/chess/king_allowed.cpp
    src/chess/makemove.cpp
    src/chess/set_fen.cpp
    src/chess/check_evasions.cpp
    src/chess/legal_captures.cpp
    src/chess/movegen.cpp
    src/chess/square_attacked.cpp
    src/chess/count_moves.cpp
    src/chess/legal_moves.cpp
    src/chess/perft.cpp
    src/chess/squares_attacked.cpp
    src/chess/get_fen.cpp
    src/chess/legal_noncaptures.cpp
    src/chess/pinned.cpp
    src/chess/undomove.cpp
)

# Add test executable
add_executable(
    swizzles-test

    tests/main.cpp
    tests/uci.cpp
    src/search/controller.cpp

    # Tests - Search
    tests/search/50moves.cpp
    tests/search/fastmate.cpp
    tests/search/mates.cpp
    tests/search/movetime.cpp
    tests/search/stalemate.cpp
    tests/search/underpromote.cpp

    # Tests - Chess
    tests/chess/checkers.cpp
    tests/chess/hash.cpp
    tests/chess/iterator.cpp
    tests/chess/passed_pawns.cpp
    tests/chess/consistency.cpp
    tests/chess/in_check.cpp
    tests/chess/legal_moves.cpp
    tests/chess/perft.cpp
    tests/chess/draw.cpp
    tests/chess/is_capture.cpp
    tests/chess/movegen.cpp
    tests/chess/pinned.cpp
    tests/chess/fen.cpp
    tests/chess/is_legal.cpp
    tests/chess/parse_move.cpp
    tests/chess/squares_attacked.cpp

    # UCI
    src/uci/moves.cpp
    src/uci/position.cpp
    src/uci/ucinewgame.cpp

    # Search
    src/search/pv.cpp
    src/search/controller.cpp
    src/search/alphabeta.cpp
    src/search/search.cpp

    # Eval
    src/eval/eval.cpp
    src/eval/phase.cpp

    # Chess
    src/chess/attackers.cpp
    src/chess/is_legal.cpp
    src/chess/predict_hash.cpp
    src/chess/checkers.cpp
    src/chess/king_allowed.cpp
    src/chess/makemove.cpp
    src/chess/set_fen.cpp
    src/chess/check_evasions.cpp
    src/chess/legal_captures.cpp
    src/chess/movegen.cpp
    src/chess/square_attacked.cpp
    src/chess/count_moves.cpp
    src/chess/legal_moves.cpp
    src/chess/perft.cpp
    src/chess/squares_attacked.cpp
    src/chess/get_fen.cpp
    src/chess/legal_noncaptures.cpp
    src/chess/pinned.cpp
    src/chess/undomove.cpp
)

set_property(TARGET swizzles-test PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE FALSE)
target_link_libraries(swizzles PUBLIC Threads::Threads)
target_link_libraries(swizzles-test PUBLIC Threads::Threads)

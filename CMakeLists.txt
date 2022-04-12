cmake_minimum_required(VERSION 3.12)

# Project
project(Swizzles VERSION 1.0 LANGUAGES CXX)

# Packages
find_package(Threads REQUIRED)

# Third party includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/)

# Flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -pedantic -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -march=native -g -DNDEBUG")

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add the executable
add_executable(
    swizzles
    src/swizzles/main.cpp
    # Eval
    src/swizzles/eval/eval.cpp
    src/swizzles/eval/pst.cpp
    # Search
    src/swizzles/search/root.cpp
    src/swizzles/search/search.cpp
    src/swizzles/search/sort.cpp
    src/swizzles/search/qsearch.cpp
    # UCI
    src/swizzles/uci/go.cpp
    src/swizzles/uci/listen.cpp
    src/swizzles/uci/moves.cpp
    src/swizzles/uci/parse.cpp
    src/swizzles/uci/position.cpp
    src/swizzles/uci/setoption.cpp
    src/swizzles/uci/ucinewgame.cpp
    # Chess
    src/chess/calculate_hash.cpp
    src/chess/captures.cpp
    src/chess/is_attacked.cpp
    src/chess/magic.cpp
    src/chess/makemove.cpp
    src/chess/movegen.cpp
    src/chess/set_fen.cpp
    src/chess/undomove.cpp
    src/chess/zobrist.cpp
)

# Add the executable
add_executable(
    test
    src/tests/main.cpp
    src/tests/chess/counters.cpp
    src/tests/chess/fen.cpp
    src/tests/chess/is_pseudolegal.cpp
    src/tests/chess/perft_shallow.cpp
    src/tests/chess/perft.cpp
    src/tests/chess/perft960.cpp
    src/tests/chess/see.cpp
    src/tests/chess/threefold.cpp
    src/tests/chess/validate.cpp
    src/tests/chess/zobrist.cpp
    src/tests/search/50moves.cpp
    src/tests/search/mates.cpp
    src/tests/search/movetime.cpp
    src/tests/search/stalemate.cpp
    src/tests/search/tactics.cpp
    src/tests/search/underpromote.cpp
    src/tests/uci/moves.cpp
    src/tests/uci/position.cpp
    src/tests/uci/quit.cpp
    # Eval
    src/swizzles/eval/eval.cpp
    src/swizzles/eval/pst.cpp
    # Search
    src/swizzles/search/root.cpp
    src/swizzles/search/search.cpp
    src/swizzles/search/sort.cpp
    src/swizzles/search/qsearch.cpp
    # Chess
    src/chess/calculate_hash.cpp
    src/chess/captures.cpp
    src/chess/get_fen.cpp
    src/chess/is_attacked.cpp
    src/chess/is_pseudolegal.cpp
    src/chess/magic.cpp
    src/chess/makemove.cpp
    src/chess/movegen.cpp
    src/chess/predict_hash.cpp
    src/chess/see.cpp
    src/chess/set_fen.cpp
    src/chess/undomove.cpp
    src/chess/zobrist.cpp
    # UCI
    src/swizzles/uci/go.cpp
    src/swizzles/uci/moves.cpp
    src/swizzles/uci/parse.cpp
    src/swizzles/uci/position.cpp
    src/swizzles/uci/setoption.cpp
    src/swizzles/uci/ucinewgame.cpp
)

# Add the executable
add_executable(
    perft
    src/tools/perft.cpp
    # Chess
    src/chess/calculate_hash.cpp
    src/chess/is_attacked.cpp
    src/chess/magic.cpp
    src/chess/makemove.cpp
    src/chess/movegen.cpp
    src/chess/set_fen.cpp
    src/chess/undomove.cpp
    src/chess/zobrist.cpp
)

# Add the executable
add_executable(
    split
    src/tools/split.cpp
    # Chess
    src/chess/calculate_hash.cpp
    src/chess/is_attacked.cpp
    src/chess/magic.cpp
    src/chess/makemove.cpp
    src/chess/movegen.cpp
    src/chess/set_fen.cpp
    src/chess/undomove.cpp
    src/chess/zobrist.cpp
)

# Add the executable
add_executable(
    bench_perft
    src/tools/bench_perft.cpp
    # Chess
    src/chess/calculate_hash.cpp
    src/chess/is_attacked.cpp
    src/chess/magic.cpp
    src/chess/makemove.cpp
    src/chess/movegen.cpp
    src/chess/set_fen.cpp
    src/chess/undomove.cpp
    src/chess/zobrist.cpp
)

# Add the executable
add_executable(
    bench_search
    src/tools/bench_search.cpp
    # Swizzles
    src/swizzles/eval/eval.cpp
    src/swizzles/eval/pst.cpp
    src/swizzles/search/root.cpp
    src/swizzles/search/search.cpp
    src/swizzles/search/qsearch.cpp
    src/swizzles/search/sort.cpp
    # Chess
    src/chess/calculate_hash.cpp
    src/chess/captures.cpp
    src/chess/is_attacked.cpp
    src/chess/magic.cpp
    src/chess/makemove.cpp
    src/chess/movegen.cpp
    src/chess/set_fen.cpp
    src/chess/undomove.cpp
    src/chess/zobrist.cpp
)

target_link_libraries(swizzles Threads::Threads)
target_link_libraries(test Threads::Threads)

set_property(TARGET swizzles PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
set_property(TARGET perft PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
set_property(TARGET split PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
set_property(TARGET bench_perft PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
set_property(TARGET bench_search PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

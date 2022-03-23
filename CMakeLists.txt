cmake_minimum_required(VERSION 3.12)

# Project
project(Swizzles VERSION 1.0 LANGUAGES CXX)

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
    test
    src/tests/main.cpp
    src/tests/chess/counters.cpp
    src/tests/chess/fen.cpp
    src/tests/chess/perft_shallow.cpp
    src/tests/chess/perft.cpp
    src/tests/chess/perft960.cpp
    src/tests/chess/threefold.cpp
    src/tests/chess/validate.cpp
    src/tests/chess/zobrist.cpp
    # Chess
    src/chess/calculate_hash.cpp
    src/chess/get_fen.cpp
    src/chess/is_attacked.cpp
    src/chess/magic.cpp
    src/chess/makemove.cpp
    src/chess/movegen.cpp
    src/chess/predict_hash.cpp
    src/chess/set_fen.cpp
    src/chess/undomove.cpp
    src/chess/zobrist.cpp
)

cmake_minimum_required(VERSION 3.20)
project(chess)

set(CMAKE_CXX_STANDARD 17)
add_executable(chess
        main.cpp Board.cpp Board.h Piece.cpp Piece.h move_utils.h helper.h Move.cpp Move.h chess_constants.h move_utils.cpp)

add_executable(perf
        perf/perf.cpp Board.cpp Board.h Piece.cpp Piece.h move_utils.h helper.h Move.cpp Move.h chess_constants.h move_utils.cpp)

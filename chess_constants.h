//
// Created by allanbs on 07/08/2023.
//
#pragma once
#ifndef CHESS_CHESS_CONSTANTS_H
#define CHESS_CHESS_CONSTANTS_H
#include <map>
#include <limits>

enum PIECE_TYPE{
        PAWN ,
        BISHOP,
        KNIGHT,
        ROOK,
        QUEEN,
        KING,
        EMPTY
};

inline const std::map<PIECE_TYPE,double> PIECE_VALUE{
    {PAWN,1},
    {KNIGHT,3},
    {BISHOP,3},
    {ROOK,5},
    {QUEEN,9},
    {KING, 0}
};

inline const std::map<char,PIECE_TYPE> femPieceMap{
    {'p',PAWN},
    {'r',ROOK},
    {'b',BISHOP},
    {'q',QUEEN},
    {'k',KING},
    {'n', KNIGHT},
    {'P',PAWN},
    {'R',ROOK},
    {'B',BISHOP},
    {'Q',QUEEN},
    {'K',KING},
    {'N', KNIGHT}
};
#endif //CHESS_CHESS_CONSTANTS_H

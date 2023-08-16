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

const int rook_table[8][8] = {
        {  0,  0,  0,  5,  5,  0,  0,  0},
        { -5,  0,  0,  0,  0,  0,  0, -5},
        { -5,  0,  0,  0,  0,  0,  0, -5},
        { -5,  0,  0,  0,  0,  0,  0, -5},
        { -5,  0,  0,  0,  0,  0,  0, -5},
        { -5,  0,  0,  0,  0,  0,  0, -5},
        {  5, 10, 10, 10, 10, 10, 10,  5},
        {  0,  0,  0,  0,  0,  0,  0,  0}
};

const int bishop_table[8][8] = {
        {-20,-10,-10,-10,-10,-10,-10,-20},
        {-10,  5,  0,  0,  0,  0,  5,-10},
        {-10, 10, 10, 10, 10, 10, 10,-10},
        {-10,  0, 10, 10, 10, 10,  0,-10},
        {-10,  5,  5, 10, 10,  5,  5,-10},
        {-10,  0,  5, 10, 10,  5,  0,-10},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-20,-10,-10,-10,-10,-10,-10,-20}
};


const int knight_table[8][8] = {
        {-50,-40,-30,-30,-30,-30,-40,-50},
        {-40,-20,  0,  5,  5,  0,-20,-40},
        {-30,  5, 10, 15, 15, 10,  5,-30},
        {-30,  0, 15, 20, 20, 15,  0,-30},
        {-30,  5, 15, 20, 20, 15,  5,-30},
        {-30,  0, 10, 15, 15, 10,  0,-30},
        {-40,-20,  0,  0,  0,  0,-20,-40},
        {-50,-40,-30,-30,-30,-30,-40,-50}
};

const int pawn_table[8][8] = {
        {  0,  0,  0,  0,  0,  0,  0,  0},
        { 50, 50, 50, 50, 50, 50, 50, 50},
        { 10, 10, 20, 30, 30, 20, 10, 10},
        {  5,  5, 10, 25, 25, 10,  5,  5},
        {  0,  0,  0, 20, 20,  0,  0,  0},
        {  5, -5,-10,  0,  0,-10, -5,  5},
        {  5, 10, 10,-20,-20, 10, 10,  5},
        {  0,  0,  0,  0,  0,  0,  0,  0}
};








#endif //CHESS_CHESS_CONSTANTS_H

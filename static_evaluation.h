//
// Created by allanbs on 12/08/2023.
//
#pragma once

#include "chess_constants.h"

class Board;
#ifndef CHESS_STATIC_EVALUATION_H
#define CHESS_STATIC_EVALUATION_H

double calculateMaterialScore(const Board& board);
double getPieceSquare(unsigned int x,unsigned int y, PIECE_TYPE type);
double calculatePieceSquare(const Board& board);



#endif //CHESS_STATIC_EVALUATION_H

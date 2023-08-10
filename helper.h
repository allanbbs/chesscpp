//
// Created by allanbs on 06/08/2023.
//
#pragma once
#ifndef CHESS_HELPER_H
#define CHESS_HELPER_H

#include "chess_constants.h"

std::string print_bool(bool x);

std::string toChessNotation(unsigned int x, unsigned int y,PIECE_TYPE promotionType = EMPTY);

#endif //CHESS_HELPER_H

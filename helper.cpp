//
// Created by allanbs on 09/08/2023.
//
#include <string>
#include "helper.h"

std::string print_bool(bool x) {
    return x ? "true" : "false";
}

std::string toChessNotation(unsigned int x, unsigned int y, PIECE_TYPE promotionType) {
    char file = 'a' + y; // y represents columns, so it's used to determine the file
    int rank = 8 - x; // x represents rows, so it's used to determine the rank
    std::string promotion;
    if (promotionType != EMPTY) {
        switch (promotionType) {
            case QUEEN:
                promotion = 'q';
                break;

            case BISHOP:
                promotion = 'b';
                break;
            case ROOK:
                promotion = 'r';
                break;
            case KNIGHT:
                promotion = 'n';
                break;
            default:
                break;
        }
    }
    return std::string(1, file) + std::to_string(rank) + promotion;
}
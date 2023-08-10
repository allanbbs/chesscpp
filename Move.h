//
// Created by allanbs on 06/08/2023.
//

#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H
#include <vector>
#include "chess_constants.h"
#include "Piece.h"

class Move {
public:
    Move(unsigned int x0, unsigned int y0) : x(x0), y(y0) {}
    Move(unsigned int x0, unsigned int y0, bool c) : x(x0), y(y0), castle(c) {}
    Move(unsigned int x0, unsigned int y0, PIECE_TYPE type) : x(x0),y(y0),promotion(true),promotion_type(type) {};
    friend bool operator==(Move m1, Move m2) {return m1.x == m2.x && m1.y == m2.y;}
    friend std::ostream & operator<<(std::ostream &o, const Move& m) {
        o << "(" << m.x << "," << m.y << ")";
        return o;
    };
    static std::vector<Move> getPromotionMoves(unsigned int x0, unsigned int y0) {
        return {
                {x0,y0,ROOK},
                {x0,y0,KNIGHT},
                {x0,y0,BISHOP},
                {x0,y0,QUEEN},
            };
    }

    unsigned int x = 0;
    unsigned int y = 0;
    bool castle = false;
    bool enPassantExposed = false;
    bool enPassantCapture = false;
    bool promotion = false;
    PIECE_TYPE promotion_type = EMPTY;
    unsigned int captureX = 0;
    unsigned int captureY = 0;
};


#endif //CHESS_MOVE_H

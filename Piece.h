//
// Created by allanbs on 05/08/2023.
//

#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H
#include <iostream>
#include "chess_constants.h"

/*
 * if weird stuff starts happening (circular linking and such, define the PIECE_TYPE here again
 */

class Piece {
public:
    explicit Piece(PIECE_TYPE type,int p) : type(type),player(p){}
    Piece();
    Piece(const Piece& p);
    PIECE_TYPE getType() const;
    void setType(PIECE_TYPE t) { type = t;}
    friend std::ostream & operator<<(std::ostream &o, const Piece& p);
    int getPlayer() const;

    bool isEnPassantExposed() const;

    void setEnPassantExposed(bool enPassantExposed);

    bool isUnderAttack() const;

    void setUnderAttack(bool underAttack);

    bool isPinned() const;

    void setPinned(bool isPinned);

    bool hasMoved() const;

    void setMoved(bool m);

private:
    PIECE_TYPE type;
    int player;
    bool enPassantExposed = false;
    bool underAttack = false;
    bool pinned = false;
    bool moved = false;
};


#endif //CHESS_PIECE_H

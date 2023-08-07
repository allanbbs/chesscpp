//
// Created by allanbs on 05/08/2023.
//

#include "Piece.h"

PIECE_TYPE Piece::getType() const {
    return type;
}

std::ostream& operator<<(std::ostream &o, const Piece& p) {
    std::string c;
    switch (p.getType()) {
        case PAWN:
            c = p.isEnPassantExposed()? "X" : "P";
            break;
        case BISHOP:
            c = "B";
            break;
        case ROOK:
            c = "R";
            break;
        case KNIGHT:
            c = "N";
            break;
        case QUEEN:
            c = "Q";
            break;
        case KING:
            c = "K";
            break;
        case EMPTY:
            c = "0";
            break;
    }
    o << c;
    return o;
}

int Piece::getPlayer() const {
    return player;
}

Piece::Piece() {
    type =  EMPTY;
    player = 0;
}

bool Piece::isEnPassantExposed() const {
    return enPassantExposed;
}

void Piece::setEnPassantExposed(bool enPassantExposed) {
    Piece::enPassantExposed = enPassantExposed;
}

bool Piece::isUnderAttack() const {
    return underAttack;
}

void Piece::setUnderAttack(bool underAttack) {
    Piece::underAttack = underAttack;
}

bool Piece::isPinned() const {
    return pinned;
}

bool Piece::hasMoved() const {
    return moved;
}

void Piece::setMoved(bool m) {
    moved = m;
}

void Piece::setPinned(bool isPinned) {
    pinned = isPinned;
}

Piece::Piece(const Piece &p) {
    type = p.type;
    player = p.player;
    enPassantExposed = p.enPassantExposed;
    underAttack = p.underAttack;
    pinned = p.pinned;
    moved = p.moved;
}

//
// Created by allanbs on 05/08/2023.
//

#ifndef CHESS_MOVE_UTILS_H
#define CHESS_MOVE_UTILS_H

#include <tuple>
#include "chess_constants.h"

// if returns true loop must continue
bool verifyRookPosition(std::vector<Move> &r,const Piece& rook, const Piece& other,unsigned int x,unsigned int y) {
    if(other.getType() == EMPTY) {
        r.emplace_back(x,y);
        return true;
    }
    else if(other.getType() != EMPTY && other.getPlayer() != rook.getPlayer()) {
        r.emplace_back(x,y);
        return false;
    }
    else
        return false;
}

void getRookMoves(std::vector<Move> &result,const Piece& rook,const std::vector<std::vector<Piece>> &board,unsigned int x, unsigned int y) {
    //DOWN
    for(int row = x+1; row < 8; row++) {
        if (!verifyRookPosition(result,rook,board.at(row).at(y),row,y))
            break;
    }
    //UP
    for(int row = x-1; row >= 0; row--) {
        if (!verifyRookPosition(result,rook,board.at(row).at(y),row,y))
            break;
    }
    //LEFT
    for(int col = y-1; col >= 0; col--) {
        if (!verifyRookPosition(result,rook,board.at(x).at(col),x,col))
            break;
    }
    //RIGHT
    for(int col = y+1; col < 8; col++) {
        if (!verifyRookPosition(result,rook,board.at(x).at(col),x,col))
            break;
    }
}


bool verifyBishopPosition(std::vector<Move> &r,const Piece& bishop, const Piece& other,unsigned int x,unsigned int y) {
    if(other.getType() == EMPTY) {
        r.emplace_back(x,y);
        return true;
    }
    else if(other.getType() != EMPTY && other.getPlayer() != bishop.getPlayer()) {
        r.emplace_back(x,y);
        return false;
    }
    else
        return false;
}

void getBishopMoves(std::vector<Move> &result,const Piece& bishop,const std::vector<std::vector<Piece>> &board,unsigned int x, unsigned int y) {
    //DOWN-RIGHT

    for(int i = x+1,j = y+1; j < 8 && i < 8; i++,j++){
        if (not verifyBishopPosition(result,bishop,board.at(i).at(j),i,j)) {
            break;
        }
    }
    //DOWN-LEFT
    for(int i = x+1,j = y-1;j >= 0 &&  i < 8; i++,j--){
        if (not verifyBishopPosition(result,bishop,board.at(i).at(j),i,j)) {
            break;
        }
    }
    //UP-LEFT
    for(int i = x-1,j = y-1;j>= 0 && i >=0 && i < 8; i--,j--){
        if (not verifyBishopPosition(result,bishop,board.at(i).at(j),i,j)) {
            break;
        }
    }
    //UP-RIGHT
    for(int i = x-1,j = y+1; j < 8 && i >=0 && i < 8; i--,j++){
        if (not verifyBishopPosition(result,bishop,board.at(i).at(j),i,j)) {
            break;
        }
    }
}


bool verifyKnightPosition(std::vector<Move> &r,const Piece& knight, const Piece& other,unsigned int x,unsigned int y) {
    if(other.getType() == EMPTY) {
        r.emplace_back(x,y);
        return true;
    }
    else if(other.getType() != EMPTY && other.getPlayer() != knight.getPlayer()) {
        r.emplace_back(x,y);
        return false;
    }
    else
        return false;
}


void getKnightMoves(std::vector<Move> &result,const Piece& knight,const std::vector<std::vector<Piece>> &board,unsigned int x, unsigned int y) {
    std::vector<std::pair<int,int>> possibleMoves = {
            {x+1,y-2},
            {x+2,y-1},
            {x+2,y+1},
            {x+1,y+2},
            {x-1,y-2},
            {x-2,y-1},
            {x-2,y+1},
            {x-1,y+2}
    };

    for(auto& pair : possibleMoves){
        if(pair.first >= 0 && pair.first < 8 && pair.second >= 0 && pair.second < 8) {
            verifyKnightPosition(result,knight,board.at(pair.first).at(pair.second),pair.first,pair.second);
        }
    }
}

bool verifyKingPosition(std::vector<Move> &r,const Piece& king, const Piece& other,unsigned int x,unsigned int y) {
    if(other.getType() == EMPTY) {
        r.emplace_back(x,y);
        return true;
    }
    else if(other.getPlayer() != EMPTY && other.getPlayer() != king.getPlayer()) {
        r.emplace_back(x,y);
        return false;
    }
    else
        return false;
}


void getKingMoves(std::vector<Move> &result,const Piece& king,const std::vector<std::vector<Piece>> &board,unsigned int x, unsigned int y, bool withCastle = true) {
    std::vector<std::pair<int,int>> possibleMoves = {
            {x+1,y},
            {x-1,y},
            {x,y+1},
            {x,y-1},
            {x-1,y-1},
            {x-1,y+1},
            {x+1,y+1},
            {x+1,y-1}
    };

    //normal moves
    for(auto& pair : possibleMoves){
        if(pair.first >= 0 && pair.first < 8 && pair.second >= 0 && pair.second < 8) {
            verifyKingPosition(result,king,board.at(pair.first).at(pair.second),pair.first,pair.second);
        }
    }
    if(!withCastle)
        return;
    //castling
    if(king.hasMoved() && !king.isUnderAttack()) // only a king that has not moved and is not under Attack can castle
        return;

    bool foundRookFirst = true;
    int col = 0;
    for(col = y+1; col < 8 ; col++) {
        Piece temp = board.at(x).at(col);
        if(temp.getType() != EMPTY && temp.getType() != ROOK){
            foundRookFirst = false;
            break;
        }
        else if(temp.getType() == ROOK)
            break;
    }
    if(col < 8 && foundRookFirst && !board.at(x).at(col).hasMoved())
        result.emplace_back(x,col,true);
    foundRookFirst = true;
    for(col = y-1; col >= 0 ; col--) {
        Piece temp = board.at(x).at(col);
        if(temp.getType() != EMPTY && temp.getType() != ROOK){
            foundRookFirst = false;
            break;
        }
        else if(temp.getType() == ROOK)
            break;
    }
    if(col>= 0 && foundRookFirst && !board.at(x).at(col).hasMoved())
        result.emplace_back(x,col,true);

}

bool verifyPawnPosition(const std::vector<std::vector<Piece>>& board,std::vector<Move> &r,const Piece& pawn, const Piece& other,unsigned int x,unsigned int y,bool isDiagonal,bool doubleMove,int enPassantOffset) {
    Move m = Move(x,y);
    if(other.getType() == EMPTY && !isDiagonal) {
        if(doubleMove && !pawn.hasMoved())
            m.enPassantExposed = true;
        r.push_back(m);
        return true;
    }
    else if(other.getType() != EMPTY && other.getPlayer() != pawn.getPlayer() && isDiagonal) {
        if(x == 0 || x == 7) {
            auto promotionMoves = Move::getPromotionMoves(x,y);
            for(auto move : promotionMoves)
                r.push_back(move);
            return false;
        }
        r.push_back(m);
        return false;
    }
    else if(other.getType() == EMPTY && isDiagonal) {
        Piece pawnToCapture = board.at(x + enPassantOffset).at(y);
        if(pawnToCapture.getType() == PAWN && pawnToCapture.getPlayer() != pawn.getPlayer() && pawnToCapture.isEnPassantExposed()){
            m.enPassantCapture = true;
            m.captureX = x + enPassantOffset;
            m.captureY = y;
            r.push_back(m);
        }
        return false;
    }
    else
        return false;
}


void getPawnMoves(std::vector<Move> &result,const Piece& pawn,const std::vector<std::vector<Piece>> &board,unsigned int x, unsigned int y) {
    int offset = pawn.getPlayer() == 1 ? -1 : 1;
    std::vector<std::tuple<int,int,bool,bool,int>> possibleMoves = {
            std::make_tuple(x+offset,y,false,false,0),
            std::make_tuple(x+offset,y+1,true,false,-offset),
            std::make_tuple(x+offset,y-1,true,false,-offset)
    };

    if((pawn.getPlayer() == 1 && x == 6) || (pawn.getPlayer() == 0 && x == 1))
        possibleMoves.emplace_back(x+ 2*offset,y,false,true,0);

    for(auto& pair : possibleMoves){
        if(std::get<0>(pair) >= 0 && std::get<0>(pair) < 8 && std::get<1>(pair) >= 0 && std::get<1>(pair) < 8) {
            verifyPawnPosition(board,result,pawn,board.at(std::get<0>(pair)).at(std::get<1>(pair)),std::get<0>(pair),std::get<1>(pair),std::get<2>(pair),std::get<3>(pair),std::get<4>(pair));
        }
    }
}
















/*
 * Below functions are not related to move generation or filtering
 */

bool canAttack(Board *board,unsigned int x0, unsigned int y0, unsigned int x1,unsigned int y1){
    Piece piece = board->getBoard().at(x0).at(y0);
    Piece attacked = board->getBoard().at(x1).at(y1);
    std::vector<Move> moves;
    // Normal approach
    /*switch (piece.getType()) {
        case PAWN:
            // Check if the pawn can attack the king's position
            getPawnMoves(moves,piece,board->getBoard(),x0,y0);
            break;
        case BISHOP:
            // Check if the bishop can attack the king's position
            getBishopMoves(moves,piece,board->getBoard(),x0,y0);
            break;
        case KNIGHT:
            // Check if the knight can attack the king's position
            getKnightMoves(moves,piece,board->getBoard(),x0,y0);
            break;
        case ROOK:
            // Check if the rook can attack the king's position
            getRookMoves(moves,piece,board->getBoard(),x0,y0);
            break;
        case QUEEN:
            // Check if the queen can attack the king's position
            getRookMoves(moves,piece,board->getBoard(),x0,y0);
            getBishopMoves(moves,piece,board->getBoard(),x0,y0);
            break;
        case KING:
            // Check if the king can attack the king's position
            getKingMoves(moves,piece,board->getBoard(),x0,y0);
            break;
        case EMPTY:
            return false;
    }
    auto it = std::find(moves.begin(),moves.end(),Move(x1,y1));
    return it != moves.end();
    */
    // Attack vector approach
    getBishopMoves(moves,attacked,board->getBoard(),x1,y1);
    for(Move m: moves){
        Piece temp = board->getBoard().at(m.x).at(m.y);
        if((temp.getType() == QUEEN || temp.getType() == BISHOP) && temp.getPlayer() != attacked.getPlayer()){
            return true;
        }
        else if(temp.getType() == PAWN){
            if(temp.getPlayer() != attacked.getPlayer() && (abs((int)m.x-(int)x1)==1 && abs((int)m.y-(int)y1) == 1)){
                return true;
            }
        }
    }
    moves.clear();

    getRookMoves(moves,attacked,board->getBoard(),x1,y1);
    for(Move m: moves){
        Piece temp = board->getBoard().at(m.x).at(m.y);
        if((temp.getType() == QUEEN || temp.getType() == ROOK) && temp.getPlayer() != attacked.getPlayer()){
            return true;
        }
    }
    moves.clear();

    getKnightMoves(moves,attacked,board->getBoard(),x1,y1);
    for(Move m: moves){
        Piece temp = board->getBoard().at(m.x).at(m.y);
        if((temp.getType() == KNIGHT) && temp.getPlayer() != attacked.getPlayer()){
            return true;
        }
    }
    moves.clear();

    getKingMoves(moves,attacked,board->getBoard(),x1,y1, false);
    for(Move m: moves){
        Piece temp = board->getBoard().at(m.x).at(m.y);
        if(temp.getType() == PAWN){
            if(temp.getPlayer() != attacked.getPlayer() && (abs((int)m.x-(int)x1)==1 && abs((int)m.y-(int)y1) == 1)){
                if(temp.getPlayer() == 1) {
                    if(x1 < m.x)
                        return true;
                }
                else{
                    if(x1 > m.x)
                        return true;
                }

            }
        }
    }

    return false;
}

bool verifyCheck(Board *board,int player) {
    auto kingPosition = player == 0 ? board->getBlackKingPosition() : board->getWhiteKingPosition();
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++) {
            Piece temp = board->getBoard().at(i).at(j);
            if(temp.getType() != EMPTY && temp.getPlayer() != board->getBoard().at(kingPosition.first).at(kingPosition.second).getPlayer()) {
                if(canAttack(board,i,j,kingPosition.first,kingPosition.second)){
                        return true;
                }
            }
        }
    }
    return false;
}




void filterInCheckMoves(std::vector<Move>& result, Board& b,unsigned int x, unsigned int y) {
    for (auto it = result.begin(); it != result.end();) {
        int oldTurn = b.getTurn();
        auto oldBoard = b.getBoard();
        auto oldW = b.getWhiteKingPosition();
        auto oldB = b.getBlackKingPosition();
        bool moved = b.moveWithoutVerifying(x,y,*it);
        if (verifyCheck(&b,b.getTurn() == 1? 0 : 1)) {
            it = result.erase(it);
        } else {
            ++it;
        }
        b.setTurn(oldTurn);
        b.setBoard(oldBoard);
        b.setBlackKingPosition(oldB);
        b.setWhiteKingPosition(oldW);

    }
}







#endif //CHESS_MOVE_UTILS_H

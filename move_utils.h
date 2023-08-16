//
// Created by allanbs on 05/08/2023.
//

#ifndef CHESS_MOVE_UTILS_H
#define CHESS_MOVE_UTILS_H

#include <tuple>
#include "chess_constants.h"
#include "Piece.h"
#include "Move.h"
#include "Board.h"


bool verifyRookPosition(std::vector<Move> &r,const Piece& rook, const Piece& other,unsigned int x,unsigned int y);
void getRookMoves(std::vector<Move> &result,const Piece& rook,const std::vector<std::vector<Piece>> &board,unsigned int x, unsigned int y);


bool verifyBishopPosition(std::vector<Move> &r,const Piece& bishop, const Piece& other,unsigned int x,unsigned int y);
void getBishopMoves(std::vector<Move> &result,const Piece& bishop,const std::vector<std::vector<Piece>> &board,unsigned int x, unsigned int y);


bool verifyKnightPosition(std::vector<Move> &r,const Piece& knight, const Piece& other,unsigned int x,unsigned int y) ;
void getKnightMoves(std::vector<Move> &result,const Piece& knight,const std::vector<std::vector<Piece>> &board,unsigned int x, unsigned int y);


bool verifyPawnPosition(const std::vector<std::vector<Piece>>& board,std::vector<Move> &r,const Piece& pawn, const Piece& other,unsigned int x,unsigned int y,bool isDiagonal,bool doubleMove,int enPassantOffset);
void getPawnMoves(std::vector<Move> &result,const Piece& pawn,const std::vector<std::vector<Piece>> &board,unsigned int x, unsigned int y);


bool verifyKingPosition(std::vector<Move> &r,const Piece& king, const Piece& other,unsigned int x,unsigned int y);
inline bool isWithinBounds(unsigned int x, unsigned int y);
void checkCastling(std::vector<Move> &result, const Piece& king, const Board &board, unsigned int x, unsigned int y, int direction);
void getKingMoves(std::vector<Move> &result,const Piece& king,const Board &board,unsigned int x, unsigned int y, bool withCastle = true);
void getKingMovesBasic(std::vector<Move> &result,const Piece& king,const std::vector<std::vector<Piece>> &board,unsigned int x, unsigned int y);

/*
 * Below functions are not related to move generation or filtering
 */

bool canAttack(const std::vector<std::vector<Piece>>& board,
               const std::vector<Move>& bishopMoves,
               const std::vector<Move>& rookMoves,
               const std::vector<Move>& knightMoves,
               const std::vector<Move>& kingMoves,
               Piece attacked,
               unsigned int x1,
               unsigned int y1
               );

bool verifyCheck(Board *board,int player);
bool verifyCheck(const std::vector<std::vector<Piece>>& board,unsigned int x0,unsigned int y0,int player);

void filterInCheckMoves(std::vector<Move>& result, Board& b,unsigned int x, unsigned int y);







#endif //CHESS_MOVE_UTILS_H

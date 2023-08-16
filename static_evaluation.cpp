//
// Created by allanbs on 12/08/2023.
//
#include "static_evaluation.h"
#include "chess_constants.h"
#include "Board.h"

double calculateMaterialScore(const Board& board) {
    double boardScore = 0;
    unsigned int whiteBishopCount = 0;
    unsigned int blackBishopCount = 0;
    for(auto& row : board.getBoard()) {
        for(auto& piece : row) {
            if(piece.getType() == EMPTY)
                continue;
            double value = PIECE_VALUE.at(piece.getType());
            if(piece.getType() == BISHOP){
                if(piece.getPlayer() == 1)
                    whiteBishopCount++;
                else
                    blackBishopCount++;
            }
            if(piece.getPlayer() == 1) {
                boardScore += value;
            } else {
                boardScore -= value;
            }
        }
    }
    if(whiteBishopCount == 2)
        boardScore += 0.5;
    if(blackBishopCount == 2)
        boardScore -= 0.5;
    return boardScore;
}
double getPieceSquare(unsigned int x,unsigned int y, PIECE_TYPE type) {
    switch (type) {
        case PAWN:
            return pawn_table[x][y];
        case BISHOP:
            return bishop_table[x][y];
        case KNIGHT:
            return knight_table[x][y];
        case ROOK:
            return rook_table[x][y];
        case QUEEN:
            return 0;
        case KING:
            return 0;
    }
    return 0;
}
double calculatePieceSquare(const Board& b) {
    double score = 0;
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            Piece temp = b.getBoard().at(x).at(y);
            if(temp.getType() == EMPTY)
                continue;
            if (temp.getPlayer() == 1) {
                score += getPieceSquare(x,y,temp.getType())/10;
            } else {
                score += -getPieceSquare(7 - x,y,temp.getType())/10;
            }
        }

    }
    return score;

}


//
// Created by allanbs on 05/08/2023.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <vector>
#include "Piece.h"
#include "Move.h"

class Board {
public:

    Board();

    Board(const Board &b);



    std::vector<Move> getLegalMoves(unsigned int x, unsigned int y) const;

    bool move(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, PIECE_TYPE promotionType = EMPTY);
    Board moveCopy(unsigned int x0,unsigned int y0, Move move) const;

    void updatePieceTrackers(Piece p,unsigned int x,unsigned int y);

    const std::vector<std::vector<Piece>> &getBoard() const;

    void setBoard(const std::vector<std::vector<Piece>> &board);

    int getTurn() const;

    void setTurn(int turn);

    void display() const;

    std::string toJSON() const;

    void hideEnPassant();

    const std::pair<unsigned int, unsigned int> &getWhiteKingPosition() const;

    void setWhiteKingPosition(const std::pair<unsigned int, unsigned int> &whiteKingPosition);

    const std::pair<unsigned int, unsigned int> &getBlackKingPosition() const;

    void setBlackKingPosition(const std::pair<unsigned int, unsigned int> &blackKingPosition);

    bool isGameOver();

    bool isCheckmate() const;

    bool isStalemate() const;

    static std::pair<double, std::pair<Move,Move>> minimax(Board b,int depth, double alpha, double beta, bool isMaximizingPlayer);

    bool moveWithoutVerifying(unsigned int x0, unsigned int y0, Move m1);
private:
    void initBoard();
    std::vector<std::vector<Piece>> board;
    int turn;
    std::pair<unsigned int,unsigned int> whiteKingPosition;
    std::pair<unsigned int,unsigned int> blackKingPosition;
    bool checkmate = false;
    bool stalemate = false;
    int winner = -1;

    Move lastFrom = Move(0,0);
    Move lastTo = Move(0,0);




    double evaluate() const;


    static std::vector<std::pair<Move,Move>> generateAllMoves(const Board& b,int player);
};


#endif //CHESS_BOARD_H

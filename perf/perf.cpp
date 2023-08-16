//
// Created by allanbs on 09/08/2023.
//

#include <algorithm>
#include "../Board.h"
#include "../helper.h"

int perft(Board& board, int depth) {
    if (depth == 0) return 1;

    int nodes = 0;
    auto legalMoves = Board::generateAllMoves(board,board.getTurn());

    for (const auto& movePair : legalMoves) {
        Move move = movePair.second;
        unsigned int x0 = movePair.first.x;
        unsigned int y0 = movePair.first.y;

        Board tempBoard = board.moveCopy(x0, y0, move);
        nodes += perft(tempBoard, depth - 1);
    }

    return nodes;
}

void divide(Board& board, int depth) {
    if (depth <= 0) return;

    int totalNodes = 0;
    auto legalMoves = Board::generateAllMoves(board, board.getTurn());
    /*std::sort(legalMoves.begin(),legalMoves.end(),[](Move a, Move b){
        if (a.x < b.x) return true;
        if (a.x > b.x) return false;
        return a.y < b.y; // If x values are equal, compare by y
    });*/
    /*for(auto b: legalMoves){
        std::cout << "\t" << toChessNotation(b.first.x,b.first.y,EMPTY) << toChessNotation(b.second.x,b.second.y,b.second.promotion_type) << std::endl;
    }*/
    for (const auto& movePair : legalMoves) {
        Move move = movePair.second;
        unsigned int x0 = movePair.first.x;
        unsigned int y0 = movePair.first.y;

        Board tempBoard = board.moveCopy(x0, y0, move);
        int nodes = perft(tempBoard, depth - 1);
        totalNodes += nodes;

        // You can print the move and the number of nodes reached at that move
        std::cout << toChessNotation(x0,y0) << toChessNotation(move.x,move.y,move.promotion_type) << ": " << nodes << std::endl;
    }

    std::cout << "Nodes searched: " << totalNodes << std::endl;
}


int main() {

    //at start, test d1d2
    Board board = Board();
    board.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.display();
    int depth = 2;
    //(board,1);
    //divide(board,1);
    //divide(board,2);
    //divide(board,3);
    //divide(board,4);

    //std::cout << "Calculated " << perft(board,1) << " nodes!" << std::endl;
    //std::cout << "Calculated " << perft(board,2) << " nodes!" << std::endl;
    //std::cout << "Calculated " << perft(board,3) << " nodes!" << std::endl;
    //std::cout << "Calculated " << perft(board,4) << " nodes!" << std::endl;
    std::cout << "Calculated " << perft(board,5) << " nodes!" << std::endl;
    //std::cout << "Calculated " << perft(board,6) << " nodes!" << std::endl;
    //Stockfish: 4865609
    //chesscpp: 5019241
    return 0;
}
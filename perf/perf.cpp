//
// Created by allanbs on 09/08/2023.
//

#include "../Board.h"

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


int main() {

    Board board = Board();
    board.loadFEN("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 1 1");
    board.display();
    int depth = 2;
    std::cout << "Calculated " << perft(board,1) << " nodes!" << std::endl;
    std::cout << "Calculated " << perft(board,2) << " nodes!" << std::endl;
    std::cout << "Calculated " << perft(board,3) << " nodes!" << std::endl;
    std::cout << "Calculated " << perft(board,4) << " nodes!" << std::endl;
    //Stockfish: 4865609
    //chesscpp: 5019241
    return 0;
}
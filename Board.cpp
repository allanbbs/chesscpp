//
// Created by allanbs on 05/08/2023.
//
#include <algorithm>
#include <sstream>
#include <chrono>
#include "Board.h"
#include "move_utils.h"
#include "helper.h"


const std::vector<std::vector<Piece>> &Board::getBoard() const {
    return board;
}

void Board::setBoard(const std::vector<std::vector<Piece>> &board) {
    Board::board = board;
}

int Board::getTurn() const {
    return turn;
}

void Board::setTurn(int turn) {
    Board::turn = turn;
}

void Board::display() const {
    for(auto it = board.begin(); it != board.end(); it++) {
        std::string line = "";
        for(auto it2 = it->begin(); it2 != it->end(); it2++) {
            std::cout << *it2 << " ";
        }
        std::cout << '\n';
    }
    std::cout.flush();
}

Board::Board() {
    board.resize(8);
    for (auto &row : board) {
        row = std::vector<Piece>(8, Piece(EMPTY, 1));
    }
    turn = 1;
    initBoard();
}

Board::Board(Board const &b) {
    board.resize(8);
    for(int i =0; i < 8; i++){
        board.at(i) = std::vector<Piece>(8);
        for(int j = 0; j < 8; j++){
            board.at(i).at(j) = b.board.at(i).at(j);
        }
    }
    //::copy(b.board.begin(),b.board.end(),std::back_inserter(board));
    turn = b.turn;
    whiteKingPosition = b.whiteKingPosition;
    blackKingPosition = b.blackKingPosition;
    winner = b.winner;
    checkmate = b.checkmate;
    stalemate = b.stalemate;
}

void Board::initBoard() {
    //black pieces
    for(int j = 0; j < 8; j++ ) {
        board[1][j] = Piece(PAWN,0);
    }
    board[0] = {
            Piece(ROOK,0),
            Piece(KNIGHT,0),
            Piece(BISHOP,0),
            Piece(QUEEN,0),Piece(KING,0),
            Piece(BISHOP,0),
            Piece(KNIGHT,0),
            Piece(ROOK,0)
    };
    blackKingPosition = {0,4};

    //white pieces
    for(int j = 0; j < 8; j++ )
        board[6][j] = Piece(PAWN,1);

    board[7] = {
            Piece(ROOK,1),
            Piece(KNIGHT,1),
            Piece(BISHOP,1),
            Piece(QUEEN,1),Piece(KING,1),
            Piece(BISHOP,1),
            Piece(KNIGHT,1),
            Piece(ROOK,1)
    };

    whiteKingPosition = {7,4};
}

std::vector<Move> Board::getLegalMoves(unsigned int x, unsigned int y) const{
    Piece piece = board.at(x).at(y);
    std::vector<Move> result;
    switch (piece.getType()) {
        case PAWN: {
            getPawnMoves(result,piece,board,x,y);
            break;
        }

        case BISHOP: {
            getBishopMoves(result,piece,board,x,y);
            break;
        }
        case KNIGHT: {
            getKnightMoves(result,piece,board,x,y);
            break;
        }
        case ROOK: {
            getRookMoves(result,piece,board,x,y);
            break;
        }
        case QUEEN: {
            getRookMoves(result,piece,board,x,y);
            getBishopMoves(result,piece,board,x,y);
            break;
        }
        case KING: {
            getKingMoves(result,piece,board,x,y);
            break;
        }
        case EMPTY: {
            break;
        }
    }
    //checkmate = false;
    //stalemate = false;
    auto currentKingPos = turn == 1? whiteKingPosition : blackKingPosition;
    auto currentKingPiece = board.at(currentKingPos.first).at(currentKingPos.second);
    if(currentKingPiece.isUnderAttack()){ //if the current player is in check
        std::vector<Move> filteredMoves;
        std::copy_if(result.begin(),result.end(),std::back_inserter(filteredMoves),[&](Move m){
            Board temp = this->moveCopy(x,y,m);
            auto tempKingPos = temp.turn == 1? temp.blackKingPosition : temp.whiteKingPosition;
            auto tempKingPiece = temp.board.at(tempKingPos.first).at(tempKingPos.second);
            return !tempKingPiece.isUnderAttack();
        });
        return filteredMoves;
    }
    else{
        std::vector<Move> filteredMoves;
        std::copy_if(result.begin(),result.end(),std::back_inserter(filteredMoves),[&](Move m){
            Board temp = this->moveCopy(x,y,m);
            auto tempKingPos = turn == 1? temp.whiteKingPosition : temp.blackKingPosition;
            auto tempKingPiece = temp.board.at(tempKingPos.first).at(tempKingPos.second);
            return !tempKingPiece.isUnderAttack();
        });
        return filteredMoves;
    }
}

bool Board::move(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, PIECE_TYPE promotionType) {

    Piece temp = board.at(x0).at(y0);
    Piece next = board.at(x1).at(y1);
    bool hideEnPassantPawns = true;
    std::cout << "Moving " << temp << std::endl;
    std::cout << x0 << " " << y0 << " -> " << x1 << " " << y1 << std::endl;
    if(temp.getPlayer() != turn)
        return false;
    auto start = std::chrono::high_resolution_clock::now();
    auto moves = getLegalMoves(x0,y0);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
    auto move = std::find(moves.begin(),moves.end(),Move(x1,y1));
    if(move == moves.end())
        return false;
    if(move->castle) {
        Piece oldRook = next;
        board.at(x1).at(y1) = Piece();
        board.at(x0).at(y0) = Piece();

        if(x1 == 0 && y1 == 0){
            board.at(x0).at(y0-2) = temp;
            board.at(x0).at(y0-1) = oldRook;
            board.at(x0).at(y0-2).setMoved(true);
            board.at(x0).at(y0-1).setMoved(true);
            updatePieceTrackers(temp,x0,y0-2); //king
            updatePieceTrackers(temp,x0,y0-1); //rook
            lastFrom = Move(x0,y0);
            lastTo = Move(x1,y1);
        }
        else if(x1 == 0 && y1 == 7) {
            board.at(x0).at(y0+2) = temp;
            board.at(x0).at(y0+1) = oldRook;
            board.at(x0).at(y0+2).setMoved(true);
            board.at(x0).at(y0+1).setMoved(true);
            updatePieceTrackers(temp,x0,y0+2); //king
            updatePieceTrackers(temp,x0,y0+1); //rook
            lastFrom = Move(x0,y0);
            lastTo = Move(x1,y1);
        }
        else if(x1 == 7 && y1 == 0) {
            board.at(x0).at(y0-2) = temp;
            board.at(x0).at(y0-1) = oldRook;
            board.at(x0).at(y0-2).setMoved(true);
            board.at(x0).at(y0-1).setMoved(true);
            updatePieceTrackers(temp,x0,y0-2); //king
            updatePieceTrackers(temp,x0,y0-1); //rook
            lastFrom = Move(x0,y0);
            lastTo = Move(x1,y1);
        }
        else if(x1 == 7 && y1 == 7) {
            board.at(x0).at(y0+2) = temp;
            board.at(x0).at(y0+1) = oldRook;
            board.at(x0).at(y0+2).setMoved(true);
            board.at(x0).at(y0+1).setMoved(true);
            updatePieceTrackers(temp,x0,y0+2); //king
            updatePieceTrackers(temp,x0,y0+1); //rook
            lastFrom = Move(x0,y0);
            lastTo = Move(x1,y1);

        }

    }
    else {
        if(move->enPassantExposed) {
            temp.setEnPassantExposed(true);
            hideEnPassantPawns = false;
        }
        else if(move->enPassantCapture) {
            board.at(move->captureX).at(move->captureY) = Piece(EMPTY,0);
        }

        else if(move->promotion) {
            temp.setType(promotionType);
        }

        board.at(x0).at(y0) = Piece(EMPTY,0);
        board.at(x1).at(y1) = temp;
        if(!board.at(x1).at(y1).hasMoved())
            board.at(x1).at(y1).setMoved(true);
        lastFrom = Move(x0,y0);
        lastTo = Move(x1,y1);
        updatePieceTrackers(temp,x1,y1);
    }

    bool check = verifyCheck(this);
    auto opposingKingPos = turn == 1?blackKingPosition : whiteKingPosition;
    board.at(opposingKingPos.first).at(opposingKingPos.second).setUnderAttack(check);
    turn = turn == 1? 0 : 1;
    check = verifyCheck(this);
    opposingKingPos = turn == 1 ?blackKingPosition : whiteKingPosition;
    board.at(opposingKingPos.first).at(opposingKingPos.second).setUnderAttack(check);
    if(hideEnPassantPawns)
        hideEnPassant();
    moves.clear();
    return true;

}

bool Board::moveWithoutVerifying(unsigned int x0, unsigned int y0, Move move) {

    Piece temp = board.at(x0).at(y0);
    bool hideEnPassantPawns = true;
    if(temp.getPlayer() != turn)
        return false;

    if(move.castle) {
        Piece oldRook = board.at(move.x).at(move.y);
        board.at(move.x).at(move.y) = Piece();
        board.at(x0).at(y0) = Piece();

        if(move.x == 0 && move.y == 0){
            board.at(x0).at(y0-2) = temp;
            board.at(x0).at(y0-1) = oldRook;
            board.at(x0).at(y0-2).setMoved(true);
            board.at(x0).at(y0-1).setMoved(true);
            updatePieceTrackers(temp,x0,y0-2); //king
            updatePieceTrackers(temp,x0,y0-1); //rook
        }
        else if(move.x == 0 && move.y == 7) {
            board.at(x0).at(y0+2) = temp;
            board.at(x0).at(y0+1) = oldRook;
            board.at(x0).at(y0+2).setMoved(true);
            board.at(x0).at(y0+1).setMoved(true);
            updatePieceTrackers(temp,x0,y0+2); //king
            updatePieceTrackers(temp,x0,y0+1); //rook
        }
        else if(move.x == 7 && move.y == 0) {
            board.at(x0).at(y0-2) = temp;
            board.at(x0).at(y0-1) = oldRook;
            board.at(x0).at(y0-2).setMoved(true);
            board.at(x0).at(y0-1).setMoved(true);
            updatePieceTrackers(temp,x0,y0-2); //king
            updatePieceTrackers(temp,x0,y0-1); //rook
        }
        else if(move.x == 7 && move.y == 7) {
            board.at(x0).at(y0+2) = temp;
            board.at(x0).at(y0+1) = oldRook;
            board.at(x0).at(y0+2).setMoved(true);
            board.at(x0).at(y0+1).setMoved(true);
            updatePieceTrackers(temp,x0,y0+2); //king
            updatePieceTrackers(temp,x0,y0+1); //rook

        }

    }
    else {
        if(move.enPassantExposed) {
            temp.setEnPassantExposed(true);
            hideEnPassantPawns = false;
        }
        else if(move.enPassantCapture) {
            board.at(move.captureX).at(move.captureY) = Piece(EMPTY,0);
        }

        else if(move.promotion) {
            temp.setType(move.promotion_type);
        }

        board.at(x0).at(y0) = Piece(EMPTY,0);
        board.at(move.x).at(move.y) = temp;
        if(!board.at(move.x).at(move.y).hasMoved())
            board.at(move.x).at(move.y).setMoved(true);
        updatePieceTrackers(temp,move.x,move.y);
    }

    bool check = verifyCheck(this);
    auto opposingKingPos = turn == 1?blackKingPosition : whiteKingPosition;
    board.at(opposingKingPos.first).at(opposingKingPos.second).setUnderAttack(check);
    turn = turn == 1? 0 : 1;
    check = verifyCheck(this);
    opposingKingPos = turn == 1 ?blackKingPosition : whiteKingPosition;
    board.at(opposingKingPos.first).at(opposingKingPos.second).setUnderAttack(check);
    if(hideEnPassantPawns)
        hideEnPassant();
    return true;

}


std::string Board::toJSON() const {
    std::string result = "[";
    std::ostringstream st;
    st << "{\"state\":\""<< (stalemate? "stalemate" : (checkmate? "checkmate" : "progress")) << "\",";
    st << "\"winner\":"<< winner << "},";
    result += st.str();
    result += "[";
    for(int i = 0; i < 8;i++) {
        for(int j = 0; j < 8; j++) {
            Piece piece = board.at(i).at(j);
            std::ostringstream temp;
            temp << "{";
            temp << "\"player\":" << piece.getPlayer() << ",";
            temp << "\"type\":" << piece.getType() << ",";
            temp << "\"enPassantExposed\":" << print_bool(piece.isEnPassantExposed()) << ",";
            temp << "\"underAttack\":" << print_bool(piece.isUnderAttack()) << ",";
            temp << "\"pinned\":" << print_bool(piece.isPinned()) << ",";
            temp << "\"moved\":" << print_bool(piece.hasMoved());
            temp << "}";
            if(i == 7 && j == 7)
                1;
            else {
                temp << ",";
            }
            result += temp.str();
        }
    }
    result += "]]";
    return result;
}

void Board::hideEnPassant() {
    for(auto it = board.begin(); it != board.end(); it++) {
        for(auto it2 = it->begin(); it2 != it->end(); it2++) {
            if(it2->getType() == PAWN)
                it2->setEnPassantExposed(false);
        }
    }
}

void Board::updatePieceTrackers(Piece p, unsigned int x, unsigned int y) {
    switch (p.getType()) {
        case KING: {
            if(p.getPlayer() == 1)
                whiteKingPosition = {x,y};
            else
                blackKingPosition = {x,y};
            break;
        }
    }
}

const std::pair<unsigned int, unsigned int> &Board::getWhiteKingPosition() const {
    return whiteKingPosition;
}

void Board::setWhiteKingPosition(const std::pair<unsigned int, unsigned int> &whiteKingPosition) {
    Board::whiteKingPosition = whiteKingPosition;
}

const std::pair<unsigned int, unsigned int> &Board::getBlackKingPosition() const {
    return blackKingPosition;
}

void Board::setBlackKingPosition(const std::pair<unsigned int, unsigned int> &blackKingPosition) {
    Board::blackKingPosition = blackKingPosition;
}

Board Board::moveCopy(unsigned int x0, unsigned int y0, Move move) const{
    Board copy = Board(*this);
    copy.moveWithoutVerifying(x0,y0,move);
    copy.lastFrom = Move(x0,y0);
    copy.lastTo = move;
    return copy;
}

bool Board::isGameOver() {
    return isCheckmate() || isStalemate();
}

bool Board::isCheckmate() const {
    return checkmate;
}

bool Board::isStalemate() const {
    return stalemate;
}

double Board::evaluate() const{
    return 0;
}


std::pair<double,std::pair<Move,Move>> Board::minimax(Board b,int depth, double alpha, double beta, bool isMaximizingPlayer) {
    if(depth == 0)
        return {b.evaluate(),{b.lastFrom,b.lastTo}};
    if(isMaximizingPlayer) {
        double bestValue = -std::numeric_limits<double>::max();
        std::pair<Move,Move> bestMove = {Move(0,0),Move(0,0)};

        for(auto& move_pair : Board::generateAllMoves(b,1)){
            Move m1 = move_pair.first;
            Move m2 = move_pair.second;
            Board temp = b.moveCopy(m1.x,m1.y,m2);
            auto res = minimax(temp,depth-1,alpha,beta,false);
            if(res.first > bestValue){
                bestValue = res.first;
                bestMove = res.second;
            }
            alpha = std::max(alpha,bestValue);
            if(beta <= alpha)
                break;
        }
        return {bestValue,bestMove};
    }
    else{
        double bestValue = std::numeric_limits<double>::max();
        std::pair<Move,Move> bestMove = {Move(0,0),Move(0,0)};

        for(auto& move_pair : Board::generateAllMoves(b,0)){
            Move m1 = move_pair.first;
            Move m2 = move_pair.second;
            Board temp = b.moveCopy(m1.x,m1.y,m2);
            auto res = minimax(temp,depth-1,alpha,beta,true);
            if(res.first < bestValue){
                bestValue = res.first;
                bestMove = res.second;
            }
            beta = std::min(beta,bestValue);
            if(beta <= alpha)
                break;
        }
        return {bestValue,bestMove};
    }



}

std::vector<std::pair<Move,Move>> Board::generateAllMoves(const Board& b,int player) {

    std::vector<std::pair<Move,Move>> result;

    for(int i=0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            Piece temp = b.board.at(i).at(j);
            if(temp.getType() != EMPTY && temp.getPlayer() == player) {
                auto moves = b.getLegalMoves(i,j);
                for(Move m2 : moves){
                    result.emplace_back(Move(i,j),m2);
                }
            }
        }
    }
    std::cout << "--------------------------------------------------------"<< std::endl;
    for(auto p : result){
        std::cout<< p.first << " -> " << p.second << std::endl;
    }
    std::cout << "--------------------------------------------------------"<< std::endl;
    return result;

}





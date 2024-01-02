//
// Created by allanbs on 05/08/2023.
//
#include <algorithm>
#include <sstream>
#include <chrono>
#include "chess_constants.h"
#include "Board.h"
#include "move_utils.h"
#include "helper.h"
#include "static_evaluation.h"

const std::vector<std::vector<Piece>> &Board::getBoard() const {
    return board;
}

void Board::setBoard(const std::vector<std::vector<Piece>> &b) {
    board = b;
}

int Board::getTurn() const {
    return turn;
}

void Board::setTurn(int t) {
    turn = t;
}

void Board::display() const {
    std::string top = "+---+---+---+---+---+---+---+---+\n";
    int i = 8;
    for(auto it = board.begin(); it != board.end(); it++,i--) {
        std::ostringstream line;
        for(auto it2 = it->begin(); it2 != it->end(); it2++) {
            line << "| " << *it2 << " ";
        }
        line << "|  " << i << "\n";
        line << "+---+---+---+---+---+---+---+---+\n";
        top += line.str();
    }
    top += "  a   b   c   d   e   f   g   h\n";
    std::cout << top << std::endl;
}

Board::Board() {
    board.resize(8);
    for (auto &row : board) {
        row = std::vector<Piece>(8, Piece(EMPTY, 1));
    }
    turn = 1;
    initBoard();
}

void Board::loadFEN(std::string moveSequence) {
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++) {
            board.at(i).at(j) = Piece(EMPTY,0);
        }
    }
    int piecePlacement = 0, playerToMove = 1, castlingRights = 2, enPassantTarget = 3, halfmoveClock = 4,fullCounter = 5;
    std::istringstream iss(moveSequence);
    std::vector<std::string> result;
    std::string token;
    while (std::getline(iss, token, ' ')) {
        result.push_back(token);
    }
    int board1Dindex = 0;
    for(char chr : result.at(piecePlacement)) {
        if(chr == '/'){
            continue;
        }
        else if(isdigit(chr)) {
            int consecutiveEmptySpaces = chr - '0';
            for(int i = 0;i < consecutiveEmptySpaces; i++)
                board.at(board1Dindex/8).at(board1Dindex%8) = Piece();
            board1Dindex += consecutiveEmptySpaces;
        } else if(std::isalpha(chr)){
            board.at(board1Dindex/8).at(board1Dindex%8) = Piece(femPieceMap.at(chr),std::islower(chr) ? 0 : 1);
            if(chr == 'k') {
                blackKingPosition = {board1Dindex/8,board1Dindex%8};
            }
            else if(chr == 'K'){
                whiteKingPosition =  {board1Dindex/8,board1Dindex%8};
            }
            board1Dindex++;
        }
    }

    turn = result.at(playerToMove) == "w" ? 1: 0;

    canCastleQueenSide[0] = false;
    canCastleQueenSide[1] = false;
    canCastleKingSide[0] = false;
    canCastleKingSide[1] = false;
    if(result.at(castlingRights) != "-"){
        for(char c: result.at(castlingRights)) {
            if(c == 'K')
                canCastleKingSide[1] = true;
            if(c == 'Q')
                canCastleQueenSide[1] = true;
            if(c == 'k')
                canCastleKingSide[0] = true;
            if(c == 'q')
                canCastleQueenSide[0] = true;
        }
    }

}

Board::Board(Board const &b) {
    board.resize(8);
    for(int i =0; i < 8; i++){
        board.at(i) = std::vector<Piece>(8);
        for(int j = 0; j < 8; j++){
            board.at(i).at(j) = b.board.at(i).at(j);
        }
    }
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

    // TODO develop a test suite to allow checking if changes did not break any rules
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
            getKingMoves(result,piece,*this,x,y);
            break;
        }
        case EMPTY: {
            break;
        }
    }
    auto currentKingPos = turn == 1? whiteKingPosition : blackKingPosition;
    auto currentKingPiece = board.at(currentKingPos.first).at(currentKingPos.second);
    Board temp = *this; // One copy of the board, to not break the const qualifier of this function.

    if(currentKingPiece.isUnderAttack()){
        filterInCheckMoves(result, temp,x, y);
    } else {
        filterInCheckMoves(result, temp,x, y);
    }

    return result;
}

bool Board::move(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, PIECE_TYPE promotionType) {

    Piece temp = board.at(x0).at(y0);
    if(temp.getPlayer() != turn)
        return false;

    std::cout << "Moving " << temp << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    auto moves = getLegalMoves(x0,y0);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    auto move = std::find(moves.begin(),moves.end(),Move(x1,y1));

    if(move == moves.end())
        return false;

    start = std::chrono::high_resolution_clock::now();
    bool canMove = executeMove(x0,y0,*move,promotionType == EMPTY ? move->promotion_type : promotionType);
    end = std::chrono::high_resolution_clock::now();
    if(canMove) {
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Time taken to generate legal moves: " << duration1.count() << " microseconds" << std::endl;
        std::cout << "Time taken to execute move and verify check: " << duration2.count() << " microseconds" << std::endl;
        std::cout << "Time taken total: " << duration1.count() + duration2.count() << " microseconds" << std::endl;
    }
    auto allMovesRemaining = generateAllMoves(*this,turn);
    auto opposingKingPos = turn == 1 ? whiteKingPosition:blackKingPosition;
    if(allMovesRemaining.empty()) {
        if(board.at(opposingKingPos.first).at(opposingKingPos.second).isUnderAttack()) {
            std::cout<<"Checkmate"<<std::endl;
            winner = turn == 1? 0 : 1;
            checkmate = true;
        } else{
            std::cout<<"Stalemate"<<std::endl;
            stalemate = true;
        }
    }
    return canMove;

}

bool Board::moveWithoutVerifying(unsigned int x0, unsigned int y0, Move move) {

    Piece temp = board.at(x0).at(y0);
    if(temp.getPlayer() != turn)
        return false;

    bool canMove = executeMove(x0,y0,move,move.promotion_type);
    auto allMovesRemaining = generateAllMoves(*this,turn);
    auto opposingKingPos = turn == 1 ? whiteKingPosition:blackKingPosition;
    if(allMovesRemaining.empty()) {
        if(board.at(opposingKingPos.first).at(opposingKingPos.second).isUnderAttack()) {
            std::cout<<"Checkmate"<<std::endl;
            winner = turn == 1? 0 : 1;
            checkmate = true;
        } else{
            std::cout<<"Stalemate"<<std::endl;
            stalemate = true;
        }
    }
    return canMove;

}

bool Board::executeMoveWithoutVerifying(unsigned int x0, unsigned int y0, Move move) {
    Piece temp = board.at(x0).at(y0);
    if(temp.getPlayer() != turn) {
        return false;
    }

    return executeMove(x0, y0, move, move.promotion_type);
}

bool Board::executeMove(unsigned int x0,unsigned int y0, Move move, PIECE_TYPE promotionType){
    unsigned int x1 = move.x;
    unsigned int y1 = move.y;
    bool hideEnPassantPawns = true;
    Piece temp = board.at(x0).at(y0);
    Piece next = board.at(x1).at(y1);
    //TODO fix castling
    if(move.castle) {
        Piece oldRook;

        board.at(x0).at(y0) = Piece();
        board.at(x1).at(y1) = temp;
        if(y1 < y0) { //rook to left
            oldRook = board.at(x1).at(0);
            board.at(x1).at(0) = Piece();
            board.at(x1).at(y1+1) = oldRook;
        }
        else{ //rook to right
            oldRook = board.at(x1).at(7);
            board.at(x1).at(7) = Piece();
            board.at(x1).at(y1-1) = oldRook;

        }
        updatePieceTrackers(temp,x1,y1);
        lastFrom = Move(x0,y0);
        lastTo = Move(x1,y1);

        /*
        if((x1 == 0 && y1 == 0) || (x1 == 7 && y1 == 0)){
            board.at(x0).at(y0-2) = temp;
            board.at(x0).at(y0-1) = next;
            board.at(x0).at(y0-2).setMoved(true);
            board.at(x0).at(y0-1).setMoved(true);
            updatePieceTrackers(temp,x0,y0-2); //king
            //updatePieceTrackers(temp,x0,y0-1); //rook
            lastFrom = Move(x0,y0);
            lastTo = Move(x1,y1);
        }
        else if((x1 == 0 && y1 == 7) || (x1 == 7 && y1 == 7)) {
            board.at(x0).at(y0+2) = temp;
            board.at(x0).at(y0+1) = next;
            board.at(x0).at(y0+2).setMoved(true);
            board.at(x0).at(y0+1).setMoved(true);
            updatePieceTrackers(temp,x0,y0+2); //king
            //updatePieceTrackers(temp,x0,y0+1); //rook
            lastFrom = Move(x0,y0);
            lastTo = Move(x1,y1);
        }
         */

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
    //verify if current move was made if current king was in check to remove the check mark.
    auto currentKingPos = turn == 1 ? whiteKingPosition: blackKingPosition;
    auto currentKingPiece = board.at(currentKingPos.first).at(currentKingPos.second);
    if(currentKingPiece.isUnderAttack())
        currentKingPiece.setUnderAttack(false);
    //
    bool check = verifyCheck(this,turn == 1? 0 : 1);
    auto opposingKingPos = turn == 1?blackKingPosition : whiteKingPosition;
    board.at(opposingKingPos.first).at(opposingKingPos.second).setUnderAttack(check);
    turn = turn == 1? 0 : 1;
    updateCastlingRights(x0,y0,x1,y1);
    if(hideEnPassantPawns)
        hideEnPassant();
    return true;
}

std::string Board::toJSON() const {
    std::string result = "[";
    std::ostringstream st;
    st << R"({"state":")"<< (stalemate? "stalemate" : (checkmate? "checkmate" : "progress")) << "\",";
    st << "\"lastFrom\":" << lastFrom << ",";
    st << "\"lastTo\":" << lastTo << ",";
    st << "\"winner\":"<< winner << "},";
    result += st.str();
    result.replace(result.find("("),1,"[");
    result.replace(result.find(")"),1,"]");
    result.replace(result.find("("),1,"[");
    result.replace(result.find(")"),1,"]");
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
            if(!(i == 7 && j == 7))
                temp << ",";
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

void Board::updatePieceTrackers(const Piece& p, unsigned int x, unsigned int y) {
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

void Board::setWhiteKingPosition(const std::pair<unsigned int, unsigned int> &w) {
    whiteKingPosition = w;
}

const std::pair<unsigned int, unsigned int> &Board::getBlackKingPosition() const {
    return blackKingPosition;
}

void Board::setBlackKingPosition(const std::pair<unsigned int, unsigned int> &b) {
    blackKingPosition = b;
}

Board Board::moveCopy(unsigned int x0, unsigned int y0, Move move) const{
    Board copy = Board(*this);
    copy.executeMoveWithoutVerifying(x0,y0,move);
    copy.lastFrom = Move(x0,y0);
    copy.lastTo = move;
    return copy;
}

bool Board::isGameOver() const {
    return isCheckmate() || isStalemate();
}

bool Board::isCheckmate() const {
    return checkmate;
}

bool Board::isStalemate() const {
    return stalemate;
}

double Board::evaluate() const{
    double boardScore = 0;
    boardScore += calculateMaterialScore(*this);
    //boardScore += calculatePieceSquare(*this);
    if(isGameOver()) {
        if(winner == -1)
            boardScore += 0;
        else{
            if(winner == 1) {
                boardScore += 10000;
            }else{
                boardScore -= 10000;
            }
        }
        return boardScore;
    }
    return boardScore;
}


std::pair<double,std::pair<Move,Move>> Board::minimax( Board& b,int depth, double alpha, double beta, bool isMaximizingPlayer) {
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
                bestMove = {m1,m2};
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
                bestMove = {m1,m2};
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
    return result;

}

void Board::updateCastlingRights(unsigned int x0,unsigned int y0, unsigned int x1, unsigned y1) {
    Piece temp = board.at(x1).at(y1);
    if(temp.getType() == KING) {
        canCastleKingSide[temp.getPlayer()] = false;
        canCastleQueenSide[temp.getPlayer()] = false;
    }
    else if(temp.getType() == ROOK) {
        if(y0 == 0) {
            canCastleQueenSide[temp.getPlayer()] = false;
        }
        else{
            canCastleKingSide[temp.getPlayer()] = false;
        }
    }
}

bool Board::canCastleKS(int player) const{
    return canCastleKingSide[player];
}

bool Board::canCastleQS(int player) const{
    return canCastleQueenSide[player];
}







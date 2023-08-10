//
// Created by allanbs on 05/08/2023.
//
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include "Board.h"
int server_fd, new_socket;

void atexit() {

    close(new_socket);
    close(server_fd);

}

int main() {

    std::atexit(atexit);
    struct sockaddr_in address{};
    // Attach socket to the port 8080
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[24] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket failed";
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Socket options failed";
        return -1;
    }



    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed";
        return -1;
    }
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed";
        return -1;
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Accept failed";
        return -1;
    }

    Board b = Board();
    b.loadFEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    while(true) {
        ssize_t readBytes = read(new_socket, buffer, 4*6);
        if(readBytes == -1) {
            std::cerr << "Something went wrong from reading socket!" << std::endl;
            break;
        }
        int32_t x0,y0,x1,y1,player,promotionType;
        std::memcpy(&x0, buffer, sizeof(int));
        std::memcpy(&y0, buffer + sizeof(int), sizeof(int));
        std::memcpy(&x1, buffer + 2 * sizeof(int), sizeof(int));
        std::memcpy(&y1, buffer + 3 * sizeof(int), sizeof(int));
        std::memcpy(&player, buffer + 4 * sizeof(int), sizeof(int));
        std::memcpy(&promotionType, buffer + 5 * sizeof(int), sizeof(int));
        x0 = ntohl(x0);
        y0 = ntohl(y0);
        x1 = ntohl(x1);
        y1 = ntohl(y1);
        player = ntohl(player);
        promotionType = ntohl(promotionType);
        x0 = static_cast<int32_t>(x0);
        y0 = static_cast<int32_t>(y0);
        x1 = static_cast<int32_t>(x1);
        y1 = static_cast<int32_t>(y1);
        player = static_cast<int32_t>(player);
        promotionType = static_cast<int32_t>(promotionType);

        if(x1 == -1 && y1 == -1 && player == -1)
            break;
        bool canMove = b.move(x0, y0, x1, y1, static_cast<PIECE_TYPE>(promotionType));
        if(canMove && !b.isGameOver()){
            /*auto res = Board::minimax(b,4,-std::numeric_limits<double>::max(),+std::numeric_limits<double>::max(),false);
            Move m1 = res.second.first;
            Move m2 = res.second.second;
            b.moveWithoutVerifying(m1.x,m1.y,m2);
            std::cout << "(" << m1.x << "," << m1.y << ") -> (" << m2.x << "," << m2.y << ")" <<  std::endl;*/
        }
        std::string jsonString = b.toJSON();
        const char* json = jsonString.c_str();
        if(b.isGameOver()) {
            std::cout<<"Game over!"<< (b.isCheckmate()? (b.winner == 0? "Black has won!" : "White has won!") : "Stalemate!") << std::endl;
            send(new_socket, json,strlen(json) , 0);
            break;
        }
        send(new_socket, json,strlen(json) , 0);

    }
    close(new_socket);
    close(server_fd);
    std::cout << "Game finished!" << std::endl;
    return 0;
}

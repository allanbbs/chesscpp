
from view import drawer
import socket


PIECE_TYPE = {
    0:"P" ,
    1:"B",
    2:"N",
    3:"R",
    4:"Q",
    5:"K",
    6:"E"
}

board = [
    [3,2,1,4,5,1,2,3],
    [0,0,0,0,0,0,0,0],
    [6 for a in range(8)],
    [6 for a in range(8)],
    [6 for a in range(8)],
    [6 for a in range(8)],
    [0,0,0,0,0,0,0,0],
    [3,2,1,4,5,1,2,3]
]

for i in range(8):
    for j in range(8):
        board[i][j] = {"type":board[i][j],"player": 0 if i < 3 else 1}



def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Define the host and port to connect to
    host = "127.0.0.1"
    port = 8080

    # Connect to the server
    client_socket.connect((host, port))
    d = drawer.Drawer(1000,1000,client_socket)
    d.add_board(board)
    while True:

        if d.process_events():
            break
        d.print_board()
        d.update()

    

    # Close the socket
    client_socket.close()

if __name__ == "__main__":
    main()
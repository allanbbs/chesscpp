import struct
import pygame
import json
import copy

PIECE_TYPE = {
    0:"P" ,
    1:"B",
    2:"N",
    3:"R",
    4:"Q",
    5:"K",
    6:"E"
}

SPRITES = {
    1 : {
        0:pygame.transform.scale(pygame.image.load("sprites/w_pawn.png"),(125,125)) ,
        1:pygame.transform.scale(pygame.image.load("sprites/w_bishop.png"),(125,125)),
        2:pygame.transform.scale(pygame.image.load("sprites/w_knight.png"),(125,125)),
        3:pygame.transform.scale(pygame.image.load("sprites/w_rook.png"),(125,125)),
        4:pygame.transform.scale(pygame.image.load("sprites/w_queen.png"),(125,125)),
        5:pygame.transform.scale(pygame.image.load("sprites/w_king.png"),(125,125))
    },
    0: {
        0:pygame.transform.scale(pygame.image.load("sprites/b_pawn.png"),(125,125)) ,
        1:pygame.transform.scale(pygame.image.load("sprites/b_bishop.png"),(125,125)),
        2:pygame.transform.scale(pygame.image.load("sprites/b_knight.png"),(125,125)),
        3:pygame.transform.scale(pygame.image.load("sprites/b_rook.png"),(125,125)),
        4:pygame.transform.scale(pygame.image.load("sprites/b_queen.png"),(125,125)),
        5:pygame.transform.scale(pygame.image.load("sprites/b_king.png"),(125,125)) 
    },
    "black" : pygame.transform.scale(pygame.image.load("sprites/square_brown_dark.png"),(125,125)),
    "white" : pygame.transform.scale(pygame.image.load("sprites/square_brown_light.png"),(125,125))
}


class Drawer:

    def __init__(self,width,height,socket):
        self.state = None
        self.turn = 1
        self.socket = socket
        self.board = None
        self.width = width
        self.height = height
        pygame.init()
        self.font = pygame.font.SysFont("Comic Sans MS",60)
        self.window = pygame.display.set_mode((width,height))
        self.fps = pygame.time.Clock()
        self.board_size = None
        self.resolution = None
        self.selected = None
        pygame.display.set_caption("Chess")

    def add_board(self,board):
        self.board = board
        self.board_size = len(board)
        self.resolution = 1000//self.board_size
            

    def update(self):
        self.fps.tick(30)
        pygame.display.flip()
        pygame.event.pump()

    def process_events(self):
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE or event.key == pygame.K_q:
                    integers = [-1,-1,-1,-1,-1,-1]
                    # Pack the integers into a binary format
                    message = struct.pack('!6i', *integers) # ! denotes network byte order, 5I denotes five unsigned integers
                    # Send the packed message to the server
                    self.socket.sendall(message)
                    pygame.quit()
                    return True
            if event.type == pygame.MOUSEBUTTONDOWN:
                y,x = event.pos
                y //= self.resolution
                x //= self.resolution
                self.selected = None
                for i in range(8):
                    for j in range(8):
                        if x == i and j == y and self.board[i][j]["type"] != 6:
                            self.selected = (x,y)
            if event.type == pygame.MOUSEBUTTONUP:
                y,x = event.pos
                y //= self.resolution
                x //= self.resolution
                if self.selected and (0 <= x < 8) and (0 <= y < 8):
                    integers = [self.selected[0], self.selected[1], x, y, self.turn,4]
                    # Pack the integers into a binary format
                    message = struct.pack('!6i', *integers) # ! denotes network byte order, 5I denotes five unsigned integers
                    # Send the packed message to the server
                    self.socket.sendall(message)
                    oldBoard = copy.deepcopy(self.board)
                    obj = json.loads(self.socket.recv(10000).decode("utf-8"))
                    self.state = obj[0]
                    print(self.state)
                    newBoard = obj[1]
                    #newBoard = json.loads(newBoard)
                    if self.state["state"] != "progress":
                        print("Stalemate!" if self.state["state"] == "stalemate" else "Checkmate!","White" if self.state["winner"] == 1 else "Black","won!")
                        pygame.quit()
                        return True
                    for i in range(8):
                        for j in range(8):
                            self.board[i][j] = newBoard[i*8 + j]
                    if self.selected and self.board != oldBoard:
                        self.turn = 1 if self.turn == 0 else 0
                        print("Now playing:","White" if self.turn ==1 else "Black")
                    self.selected = None
                    

                self.selected = None
                

        return False

    def print_board(self):
        self.window.fill((0,0,0))
        for i in range(self.board_size):
            for j in range(self.board_size):
                x = i*self.resolution
                y = j*self.resolution
                
                #rect = pygame.Rect(y, x, self.resolution-1, self.resolution-1)
                #pygame.draw.rect(self.window,(125, 135, 150) if (i + j) % 2 == 0 else (233, 236, 239), rect)
                self.window.blit(SPRITES["white"] if (i + j) % 2 == 0 else SPRITES["black"],(y,x))
                if self.board[i][j]["type"] != 6:
                    temp = self.board[i][j]
                    type = PIECE_TYPE.get(temp["type"])
                    #letter = self.font.render(type,1,(0,255,0) if self.board[i][j]["player"] == 1 else (255,0,0))
                    self.window.blit(SPRITES[temp["player"]][temp["type"]],(y,x))
                    if self.selected and i ==self.selected[0] and j == self.selected[1]:
                        rect = pygame.Rect(y, x, self.resolution-1, self.resolution-1)
                        pygame.draw.rect(self.window,(0,0,255),rect,5)
                    if temp.get("underAttack") and temp["type"] == 5:
                        rect = pygame.Rect(y, x, self.resolution-1, self.resolution-1)
                        pygame.draw.rect(self.window,(255,0,0),rect,5)
    
                


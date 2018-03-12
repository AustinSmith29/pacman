CC = g++
CFLAGS = -g -Wall -std=c++17
CLINK = -lSDL2

default: pacman

clean:
	rm *.o pacman

pacman: game.o input.o graphics.o gamestate.o maze.o
	$(CC) $(CFLAGS) -o pacman main.cpp game.o input.o graphics.o gamestate.o maze.o $(CLINK)

game.o: Game.cpp 
	$(CC) -c -o game.o $(CFLAGS) Game.cpp $(CLINK)

input.o: Input.cpp 
	$(CC) -c -o input.o $(CFLAGS) Input.cpp $(CLINK)

graphics.o: Graphics.cpp 
	$(CC) -c -o graphics.o $(CFLAGS) Graphics.cpp $(CLINK)

gamestate.o: GameState.cpp 
	$(CC) -c -o gamestate.o $(CFLAGS) GameState.cpp $(CLINK)

maze.o: Maze.cpp 
	$(CC) -c -o maze.o $(CFLAGS) Maze.cpp $(CLINK)

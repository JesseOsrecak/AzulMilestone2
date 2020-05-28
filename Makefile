.default: all

all: azul

clean:
	rm -f azul *.o

azul: BoxLid.o Factory.o FloorLine.o GameAction.o GameEngine.o GameModel.o GameTurn.o IOHandler.o LinkedList.o Menu.o ModelBuilder.o Mosaic.o Node.o PatternLine.o Player.o PlayerBoard.o Tile.o TileBag.o main.o 
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c $^

CC=g++
CFLAGS=-std=c++11 -Wall -O2

NAME=Rogue
FILES=src/Main.cpp src/Graphics.cpp src/Architect.cpp src/Mob.cpp src/Item.cpp

all:
	$(CC) $(CFLAGS) $(FILES) -o $(NAME) -lncurses

clean:
	rm -rf *o $(NAME)

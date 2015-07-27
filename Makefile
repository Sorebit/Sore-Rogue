CC=g++
CFLAGS=-std=c++11 -Wall -O2

NAME=Rogue
FILES=Main.cpp Graphics.cpp Architect.cpp Mob.cpp Item.cpp

all:
	$(CC) $(CFLAGS) $(FILES) -o $(NAME) -lncurses

clean:
	rm -rf *o $(NAME)

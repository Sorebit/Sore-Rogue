CC=g++
CFLAGS=-std=c++11 -Wall -O2

NAME=Rogue

all:
	$(CC) $(CFLAGS) Main.cpp Graphics.cpp Architect.cpp Mob.cpp -o $(NAME) -lncurses

clean:
	rm -rf *o $(NAME)

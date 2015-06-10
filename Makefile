#all:
#	g++ -std=c++11 -Wall -O2 Main.cpp Graphics.cpp Architect.cpp Mob.cpp -o Rogue -lncurses
CC=g++
CFLAGS=-std=c++11 -Wall -O2

all:
	$(CC) $(CFLAGS) Main.cpp Graphics.cpp Architect.cpp Mob.cpp -o Rogue -lncurses

clean:
	rm -rf *o Rogue
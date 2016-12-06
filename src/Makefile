CXX      ?= g++
CXXFLAGS ?= -std=c++11 -Wall -pedantic -O2
NAME     ?= Rogue
FILES    ?= Main.cpp Graphics.cpp Architect.cpp Mob.cpp Item.cpp
LIBS     ?= -lncurses

all:
	$(CXX) $(CXXFLAGS) $(FILES) -o $(NAME) $(LIBS)

clean:
	rm -rf *.o $(NAME)
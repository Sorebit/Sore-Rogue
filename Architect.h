#ifndef ARCHITECT_H
#define ARCHITECT_H

#include "Globals.h"

void init_map(Tile map[][200]);
int count_neighbours(int y, int x, Tile map[][200]);
void step(Tile map[][200]);
void dfs(int y, int x, Tile map[][200]);
void fill(Tile map[][200]);
void borders(Tile map[][200]);
void generate_lake(Tile map[][200]);
void position_player(Tile map[][200], Character & rogue);
void generate_dungeon(Tile map[][200], Character & rogue);

#endif
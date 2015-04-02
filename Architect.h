#ifndef ARCHITECT_H
#define ARCHITECT_H

#include "Globals.h"

void init_map(Tile map[][300]);
int count_neighbours(int y, int x, Tile map[][300]);
void step(Tile map[][300]);
void dfs(int y, int x, Tile map[][300]);
void fill(Tile map[][300]);
void borders(Tile map[][300]);
void generate_lake(Tile map[][300]);
void put_stairs(Tile map[][300], Character & rogue);
void generate_dungeon(Tile map[][300], Character & rogue);
void generate_grass(Tile map[][300]);

#endif
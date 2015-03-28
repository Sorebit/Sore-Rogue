#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>

extern std::vector <char> tileset[];

extern int maxy, maxx, comp_count, max_lake, max_lake_size, comp[200][200], count[200];


struct Character
{
	int x, y, depth; 
	int health, maxhealth; 
	int strength, armor; 
	int exp, level, nlvl; 
	int nutr, maxnutr;
	int gold, keys; 
};

struct Tile
{ 
	int door_open, tile; 
	bool seen, inView; 
};

enum keyboard
{
	down = 258,
	up = 259,
	left = 260,
	right = 261,
	quit = 113,
};

enum Tiles 
{
	//In view
	zero = 0,
	path = 1,
	grass = 2,
	pit = 3,
	edge = 4,
	bridge = 5,
	wall = 6,
	water = 7,
	coast = 8,
	player = 9,
	mob = 10,
	gold = 11,
	door = 12,
	key = 13,
	stairs = 14,
	text = 15,
	//Out of view
	opath = 21,
	ograss = 22,
	opit = 23,
	oedge = 24,
	obridge = 25,
	owall = 26,
	owater = 27,
	ocoast = 28,
	ogold = 31,
	odoor = 32,
	okey = 33,
	ostairs = 34,
	//UI
	ui1 = 50,
	bui1 = 51,
	ui2 = 53,
	bui2 = 54,
	uitext = 55,
};

enum Colors 
{
	//In view
	COLOR_DARK_BLUE = 17,
	COLOR_GOLD = 18,
	COLOR_LIGHT_GREEN = 19,
	COLOR_WATER = 20,
	COLOR_LIGHT_WATER = 21,
	COLOR_BROWN = 22,
	COLOR_LIGHT_BROWN = 23,
	COLOR_GREY = 24,
	//Out of view
	COLOR_OOV_GREY = 25,
	COLOR_OOV_DARK_GREY = 26,
	COLOR_OOV_BLUE = 27,
	COLOR_OOV_WATER = 28,
	COLOR_OOV_LIGHT_WATER = 29,
	COLOR_OOV_LIGHT_GREEN = 30,
	COLOR_OOV_GOLD = 31,
	COLOR_OOV_BROWN = 32,
	COLOR_OOV_LIGHT_BROWN = 33,
	//UI
	COLOR_UI1 = 50,
	COLOR_BUI1 = 51,
	COLOR_UI2 = 52,
	COLOR_BUI2 = 53,
};

#endif
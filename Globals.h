// Sorbet - 2015
#pragma once

#include <vector>
#include "Item.h"

extern const int inf;

extern std::vector <char> tileset[];
extern std::vector <Item> items;

extern int maxy, maxx, comp_count, max_lake, max_lake_size, comp[300][300], count[300];
extern unsigned sel, offset;

enum Statuses
{
	poisoned,
	weak,
	confused,
	invisible,
};

struct Character
{
	int x, y, depth; 
	int health, maxhealth; 
	int strength;
	int attack, defense;
	int exp, level, nlvl; 
	int nutr, maxnutr;
	int silver;
	int status[4];
	Item wep_eq, arm_eq, spc_eq;
};

extern Character rogue;

struct Tile
{ 
	int tile;
	bool seen, inView;
	bool door_open, occupied;
};

extern Tile map[300][300];

enum keyboard
{
	down = 258,
	up = 259,
	left = 260,
	right = 261,
	quit = 'q',
	eq = 'e',
	enter = 10,
	plsstop = 108,
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
	omob = 30,
	ogold = 31,
	odoor = 32,
	okey = 33,
	ostairs = 34,
	//UI
	ui1 = 51,
	dark_ui1 = 52,
	ui2 = 53,
	dark_ui2 = 54,
	ui3 = 55,
	dark_ui3 = 56,
	uitext = 57,
	eq1 = 58,
};

enum Colors 
{
	//In view
	COLOR_DARK_BLUE = 17,
	COLOR_GOLD = 18,
	COLOR_LIGHT_GREEN = 19,
	COLOR_WATER_FORE = 20,
	COLOR_WATER_BACK = 21,
	COLOR_COAST = 22,
	COLOR_BROWN = 23,
	COLOR_LIGHT_BROWN = 24,
	COLOR_GREY = 25,
	//Out of view
	COLOR_OOV_GREY = 26,
	COLOR_OOV_DARK_GREY = 27,
	COLOR_OOV_BLUE = 28,
	COLOR_OOV_WATER_FORE = 29,
	COLOR_OOV_WATER_BACK = 30,
	COLOR_OOV_COAST = 31,
	COLOR_OOV_LIGHT_GREEN = 32,
	COLOR_OOV_GOLD = 33,
	COLOR_OOV_BROWN = 34,
	COLOR_OOV_LIGHT_BROWN = 35,
	//UI
	COLOR_UI1 = 50,
	COLOR_BUI1 = 51,
	COLOR_UI2 = 52,
	COLOR_BUI2 = 53,
	COLOR_UI3 = 54,
	COLOR_BUI3 = 55,
	COLOR_EQ = 56,
};
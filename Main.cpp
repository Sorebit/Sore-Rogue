#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <cmath>

#include "Graphics.h"

int map_mx, map_maxy;
std::vector <char> tileset[] = {{'?'}, {'.'}, {'"'}, {':'}, {'.'}, {'='}, {'#'}, {'~', '-'}, {'.'}, {'@'}, {'&'}, {'o'}, {'+', '.'}, {'1'}}; 

Character rogue;

Disp display;

Tile map[200][200];

bool isPath(int x, int y)
{
	if(x < 0 || x > map_mx - 1 || y < 0 || y > display.h - 1)
		return false;
	else if(map[y][x].tile == zero || map[y][x].tile == wall || map[y][x].tile == pit || map[y][x].tile == water)
		return false;
	else if(map[y][x].tile == door)
		return (rogue.keys || map[y][x].door_open);
	return true;
}

int getMovement()
{
	int key = getch();
	switch(key)
	{
	case quit:
		return 0;
		break;
	case up:
		if(isPath(rogue.x, rogue.y - 1))
			rogue.y--;
		break;
	case down:
		if(isPath(rogue.x, rogue.y + 1))
			rogue.y++;
		break;
	case left:
		if(isPath(rogue.x - 1, rogue.y))
			rogue.x--;
		break;
	case right:
		if(isPath(rogue.x + 1, rogue.y))
			rogue.x++;
		break;
	}
	return 1;
}

void rect(int y, int x, int h, int w, int t1, int t2, bool corners = true, int t3 = 0)
	{
		for(int i = y; i < y + h; i++)
		{
			for(int j = x; j < x + w; j++)
			{
				if(i == y || i == y + h - 1 || j == x || j == x + w - 1)
					map[i][j].tile = t2;
				else
					map[i][j].tile = t1;
			}
		}
		if(!corners)
			map[y][x].tile = map[y][x + w - 1].tile = map[y + h - 1][x].tile = map[y + h - 1][x + w - 1].tile = t3;
	}

void entities()
{
	switch(map[rogue.y][rogue.x].tile)
	{
		case gold:
			rogue.gold++;
			map[rogue.y][rogue.x].tile = path;
			break;
		case key:
			rogue.keys++;
			map[rogue.y][rogue.x].tile = path;
			break;
		case door:
			if(!map[rogue.y][rogue.x].door_open && rogue.keys)
			{
				map[rogue.y][rogue.x].door_open = true;
				rogue.keys--;
			}
			break;
	}
}

void render_player()
{
	if(map[rogue.y][rogue.x].tile == edge)
		init_pair(player, COLOR_WHITE, COLOR_BLUE);
	else if(map[rogue.y][rogue.x].tile == door || map[rogue.y][rogue.x].tile == bridge)
		init_pair(player, COLOR_WHITE, COLOR_BROWN);
	else
		init_pair(player, COLOR_WHITE, COLOR_DARK_BLUE);
	attron(COLOR_PAIR(player));
	mvprintw(rogue.y, rogue.x + 25, "@");
}

void render()
{
	for(int i = 0; i < display.h; i++)
	{
		for(int j = 0; j < map_mx; j++)
		{
			map[i][j].inView = false;
		}
	}
	for(int i = 0; i < display.h; i++)
	{
		for(int j = 0; j < map_mx; j++)
		{
			if(!i || i == display.h - 1 || !j || j == map_mx - 1)
			{
				ray(map, rogue.x, rogue.y, j, i);
			}
		}
	}
	for(int i = 0; i < display.h; i++)
	{
		for(int j = 0; j < map_mx; j++)
		{
			if(!map[i][j].tile)
				continue;
			if(!map[i][j].seen)
				continue;
			if(map[i][j].inView)
				attron( COLOR_PAIR(map[i][j].tile) );
			else
				attron(COLOR_PAIR(map[i][j].tile + 20));
			switch(map[i][j].tile)
			{
				case door:
					mvprintw(i, j + 25, "%c", tileset[door][ map[i][j].door_open ]);
					break;
				default:
					mvprintw(i, j + 25, "%c", tileset[ map[i][j].tile ][rand() % tileset[ map[i][j].tile ].size() ]);
			}
			
		}
	}
	render_player();
}



void lighting_test()
{
	rogue.y = 21;
	rogue.x = 43;
	rect(10, 20, 23, 47, path, wall);
	rect(13, 30, 3, 6, zero, wall);
	rect(13, 51, 3, 6, zero, wall);
	rect(20, 30, 3, 6, zero, wall);
	rect(20, 51, 3, 6, zero, wall);
	rect(27, 30, 3, 6, zero, wall);
	rect(27, 51, 3, 6, zero, wall);
	return;
}

void test_map()
{
	rogue.y = 21;
	rogue.x = 48;
	rogue.keys = 0;
	rogue.level = 6;
	rogue.exp = 65;
	rogue.nlvl = 100;
	rogue.strength = 13;
	rogue.armor = 8;
	rogue.maxhealth = 10;
	rogue.health = 5; 
	rogue.nutr = 6;
	rogue.maxnutr = 15;
	rogue.gold = 657;
	rogue.depth = 5;

	//komora testowa 1
	rect(8, 10, 15, 30, path, wall);

	//komora testowa 2
	rect(8, 45, 15, 30, path, wall);
	//komora testowa 3
	rect(25, 45, 15, 30, path, wall);
	rect(28, 46, 11, 23, edge, edge);
	rect(29, 46, 10, 22, pit, pit);
	rect(28, 51, 4, 1, bridge, bridge);
	rect(28, 55, 4, 1, bridge, bridge);
	rect(31, 51, 1, 5, bridge, bridge);
	rect(31, 53, 4, 1, bridge, bridge);
	rect(35, 60, 3, 1, bridge, bridge);
	rect(35, 53, 1, 16, bridge, bridge);
	rect(37, 61, 1, 8, bridge, bridge);
	//komora testowa 4
	rect(25, 80, 15, 30, path, wall);

	//przejscie 1 do 2
	rect(13, 39, 5, 7, path, wall);
	rect(14, 39, 3, 1, path, path);

	//przejscie 2 do 3
	rect(22, 55, 4, 10, path, wall);
	rect(22, 56, 1, 8, path, path);
	rect(25, 56, 1, 8, path, path);

	//przejscie z 3 do 4
	rect(30, 74, 5, 7, path, wall);

	//staw
	rect(9, 12, 7, 12, coast, coast, false, path);
	rect(10, 13, 5, 10, water, water, false, coast);
	rect(10, 59, 7, 14, grass, grass, false, path);
	map[20][33].tile = map[20][34].tile = map[19][33].tile = map[20][32].tile = gold;
	map[15][60].tile = map[14][59].tile = map[12][68].tile = map[16][65].tile = grass;

	//klucze i drzwi
	map[19][28].tile = map[32][77].tile = map[37][70].tile = key;
	map[32][74].tile = map[15][45].tile = map[32][80].tile = door;
}

int main()
{	
	wininit();
	srand(time(NULL));
	getmaxyx(stdscr, display.h, display.w);
	map_mx = display.w - 25;
	keypad(stdscr, true);
	graphics_init();

	test_map();
	//lighting_test();
	
	printw("Press any key to start");
	while(getMovement())
	{
		entities();
		render();
		ui(display, rogue);
	}

	endwin();
	return 0;
}
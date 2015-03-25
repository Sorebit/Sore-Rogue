#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <cmath>

#include "Graphics.h"
#include "Architect.h"

std::vector <char> tileset[] = {{'?'}, {'.'}, {'"'}, {':'}, {'.'}, {'='}, {'#'}, {'~', '-'}, {'.'}, {'@'}, {'&'}, {'o'}, {'+', '.'}, {'1'}}; 

Character rogue;

Disp display;

Tile map[200][200];

bool isPath(int x, int y)
{
	if(x < 0 || x > display.map_mx - 1 || y < 0 || y > display.h - 1)
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

int main()
{	
	wininit();
	srand(time(NULL));
	getmaxyx(stdscr, display.h, display.w);
	display.map_mx = display.w - 25;
	keypad(stdscr, true);
	graphics_init();

	test_map(map, rogue);
	//lighting_test(map, rogue);
	
	printw("Press any key to start");
	while(getMovement())
	{
		entities();
		render(display, map, rogue);
		ui(display, rogue);
	}

	endwin();
	return 0;
}
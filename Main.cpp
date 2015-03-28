#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <cmath>

#include "Graphics.h"
#include "Architect.h"

std::vector <char> tileset[] = {{'?'}, {'.'}, {'"'}, {':'}, {'.'}, {'='}, {'#'}, {'~', '-'}, 
								{'.'}, {'@'}, {'&'}, {'o'}, {'+', '.'}, {'1'}, {'v'}, {'^'}}; 

int maxy, maxx, comp_count = 1, max_lake, max_lake_size, comp[200][200], count[200];

Character rogue;

Tile map[200][200];

bool isPath(int x, int y)
{
	if(x < 0 || x > maxx - 1 || y < 0 || y > maxy - 1)
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
		case stairsDown:
			clear();
			generate_dungeon(map, rogue);
			break;
	}
}

int main()
{	
	wininit();
	srand(time(NULL));
	keypad(stdscr, true);
	getmaxyx(stdscr, maxy, maxx);
	maxx -= 25;
	graphics_init();

	generate_dungeon(map, rogue);
	
	printw("Press any key to start\nQ to quit anytime.");
	while(getMovement())
	{
		entities();
		render(map, rogue);
		ui(rogue);
	}

	endwin();
	return 0;
}
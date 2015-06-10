// Sorbet - (No copyrights whatsoever) 2015
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <cmath>

#include "Graphics.h"
#include "Architect.h"
#include "Mob.h"

std::vector <char> tileset[] = {{'?'}, {'.'}, {'"'}, {':'}, {'.'}, {'='}, {'#'}, {'~', '-'}, 
								{'.'}, {'@'}, {'&'}, {'o'}, {'+', '.'}, {'1'}, {'\\'}}; 

int cy, cx, input, maxy, maxx, comp_count = 1, max_lake, max_lake_size, comp[300][300], count[300], as = 0;

std::pair <int, int> prov_steps[] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };

Character rogue;

std::vector <Mob> mob_list;

Tile map[300][300];

int mob_step = 1;

bool isPath(int x, int y)
{
	if(x < 0 || x > maxx - 1 || y < 0 || y > maxy - 1)
		return false;
	else if(map[y][x].tile == zero || map[y][x].tile == wall || map[y][x].tile == pit || map[y][x].tile == water)
		return false;
	else if(map[y][x].tile == door)
		return (rogue.keys || map[y][x].door_open);
	if(map[y][x].occupied)
		return false;
	return true;
}

void see_all()
{
	for(int y = 0; y < maxy; y++)
	{
		for(int x = 0; x < maxx; x++)
		{
			if(map[y][x].tile)
				map[y][x].seen = as;
		}
	}
	as ^= 1;
}

int getUserInput()
{
	int key = getch();

	map[rogue.y][rogue.x].occupied = false;
	switch(key)
	{
	// Special keys
	case quit:
		return 0;
		break;
	case plsstop:
		clear();
		see_all();
		break;
	// Movement keys
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
	default:
		map[rogue.y][rogue.x].occupied = true;
		return 2;
	}
	map[rogue.y][rogue.x].occupied = true;
	return 1;
}

void entities()
{
	switch(map[rogue.y][rogue.x].tile)
	{
		case gold:
			rogue.silver++;
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
		case stairs:
			clear();
			rogue.depth++;
			generate_dungeon(map, rogue);
			break;
	}
}

void mobs()
{
	for(unsigned int i = 0; i < mob_list.size(); i++)
	{
		mob_list[i].walk(prov_steps[mob_step]);
	}
	mob_step = (++mob_step == 4) ? 0 : mob_step;
}

int main()
{	
	wininit();
	srand(time(NULL));
	keypad(stdscr, true);
	getmaxyx(stdscr, maxy, maxx);
	maxx -= 25;
	graphics_init();

	printw("Press any key to start\nQ to quit anytime.\n");
	getch();
	clear();

	rogue.depth = 1;
	rogue.health = 10;
	rogue.maxhealth = 10;
	rogue.nutr = 10;
	rogue.maxnutr = 10;
	rogue.level = 1;
	rogue.strength = 12;
	rogue.armor = 3;

	generate_dungeon(map, rogue);

	Mob test_mob1(rogue, "troll", 10);
	test_mob1.setSpawn(30, 30);
	map[30][30].seen = true;
	mob_list.push_back(test_mob1);

	Mob test_mob2(rogue, "frog", 30);
	test_mob2.setSpawn(25, 25);
	map[25][25].seen = true;
	mob_list.push_back(test_mob2);

	Mob test_mob3(rogue, "witch", 40);
	test_mob3.setSpawn(20, 20);
	map[20][20].seen = true;
	mob_list.push_back(test_mob3);

	// Initial render
	mobs();
	entities();
	render(rogue, mob_list);
	ui(rogue, mob_list);

	while(true)
	{
		input = getUserInput();
		if(!input) 
			break;
		if(input == 2)
			continue;

		mobs();
		entities();
		render(rogue, mob_list);
		ui(rogue, mob_list);
	}

	endwin();
	return 0;
}
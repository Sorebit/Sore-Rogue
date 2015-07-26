// Sorbet - 2015
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

Character rogue;

std::vector <Mob> mob_list;

Tile map[300][300];

std::string messages_old[2];

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

// TODO
// Remove when finished
void see_all()
{
	for(int y = 0; y < maxy; y++)
		for(int x = 0; x < maxx; x++)
			if(map[y][x].tile)
				map[y][x].seen = as;
	as ^= 1;
}

int getUserInput()
{
	// Return values:
	// 0 - quit
	// 1 - move
	// 2 - skip frame (nothing happens)

	int key = getch();
	int dy = 0, dx = 0;

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
		dy--;
		break;
	case down:
		dy++;
		break;
	case left:
		dx--;
		break;
	case right:
		dx++;
		break;
	default:
		map[rogue.y][rogue.x].occupied = true;
		return 2;
	}
	if(isPath(rogue.x + dx, rogue.y + dy))
	{
		rogue.x += dx;
		rogue.y += dy;
		map[rogue.y][rogue.x].occupied = true;
		return 1;
	}
	else
	{
		map[rogue.y][rogue.x].occupied = true;
		return 2;
	}
	
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
	for(unsigned i = 0; i < mob_list.size(); i++)
	{	
		if(mob_list[i].seesPlayer(rogue))
		{
			mob_list[i].findPath(rogue.y, rogue.x);
			mob_list[i].follow_timeout = mob_list[i].getFollowTime();
		}
		else
		{
			if(mob_list[i].follow_timeout > 0)
				mob_list[i].follow_timeout--;

			if(!mob_list[i].follow_timeout)
				continue;
		}

		if(++mob_list[i].walk_counter >= mob_list[i].getWalkRate())
		{
			// Passive mobs
			if(mob_list[i].getFollowTime() == -1 )
			{
				mob_list[i].walk( mob_list[i].getNextStep() );
				mob_list[i].walk_counter = 0;
				continue;
			}

			// Agressive mobs
			std::string mes = "";
			int dealt = -1;
			if(mob_list[i].distFrom(rogue.y, rogue.x) == 2)
			{
				mob_list[i].walk( mob_list[i].getNextStep() );
				mob_list[i].walk_counter = 0;
				dealt = mob_list[i].attack();
				
			}
			else if(mob_list[i].distFrom(rogue.y, rogue.x) == 1)
			{
				dealt = mob_list[i].attack();
			}
			else
			{
				mob_list[i].walk( mob_list[i].getNextStep() );
				mob_list[i].walk_counter = 0;
			}

			if(dealt > 0)
				mes = mob_list[i].getName() + " " + mob_list[i].getVerb() + " you for " + std::to_string(dealt) + " hp.";
			else if(!dealt)
				mes = mob_list[i].getName() + " missed a hit";
			message(mes);
		}
	}
}

int main()
{	
	wininit();
	srand(time(NULL));

	graphics_init();

	// TODO Main menu
	printw("Welcome to the caves of your doom\n");
	printw("Press any key to start\n");
	printw("Q to quit anytime\n");
	getch();
	clear();
	
	// Hard-coded player
	rogue.depth = 1;
	rogue.health = 50;
	rogue.maxhealth = 100;
	rogue.nutr = 10;
	rogue.maxnutr = 10;
	rogue.level = 1;
	rogue.strength = 12;
	rogue.armor = 3;
	rogue.nlvl = 24;

	generate_dungeon(map, rogue);

	// Test mobs
	Mob test_mob1("troll");
	test_mob1.setSpawn(30, 30);
	map[30][30].seen = true;
	mob_list.push_back(test_mob1);
	
	Mob test_mob2("frog");
	test_mob2.setSpawn(25, 25);
	map[25][25].seen = true;
	mob_list.push_back(test_mob2);

	Mob test_mob3("witch");
	test_mob3.setSpawn(20, 20);
	map[20][20].seen = true;
	mob_list.push_back(test_mob3);
	
	// Initial render
	entities();
	render();
	ui();

	while(true)
	{
		// TODO
		// Esc menu
		// Equipment menu
		input = getUserInput();
		if(!input)
			// TODO
			// Menu 
			rogue.health = 0;
			//break;
		if(input == 2)
			continue;

		mobs();
		entities();
		render();
		ui();
		if(rogue.health <= 0)
		{
			// Maybe fade out the whole map and display something
			message("You died...");
			message("Press any key to contitnue");
			getch();
			break;
		}
	}

	endwin();
	return 0;
}

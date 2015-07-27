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

std::vector <Item> items;

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

void equipment()
{
	int _up =  maxy/2 - 10;
	int _left = (maxx + 24)/2 - 26;
	unsigned sel = 0, offset = 0, opt = 0;
	unsigned opts[] = { 0, 7, 15};
	bool esc = false;

	while(true)
	{
		attron(COLOR_PAIR(eq1));
		for(int y = _up; y < _up + 21; y++)
		{
			for(int x = _left; x < _left + 52; x++)
			{
				mvprintw(y, x, " ");
			}
		}
		mvprintw(_up + 1, _left + 22, "Equipment");

		mvprintw(_up + 3, _left + 2, "Wep: %s", rogue.wep_eq.getName().c_str());
		mvprintw(_up + 4, _left + 2, "Arm: %s", rogue.arm_eq.getName().c_str());
		mvprintw(_up + 5, _left + 2, "Spc: %s", rogue.spc_eq.getName().c_str());

		for(unsigned it = 0 + offset; it < items.size(); ++it)
		{
			if(it - offset > 8)
				break;
			std::string str = items[it].getName();
			mvprintw(_up + 7 + it - offset, _left + 4, str.c_str());
		}

		if(items.size() && sel - offset <= 8)
			mvprintw(_up + 7 + sel - offset, _left + 2, ">");

		mvprintw(_up + 17, _left + 2, "                                             ");
		mvprintw(_up + 17, _left + 2, items[sel].getInfo().c_str());
		mvprintw(_up + 19, _left + 2, "  Use    Toss    Sort");
		mvprintw(_up + 19, _left + 2 + opts[opt], ">");

		int key = getch();
		switch(key)
		{
		case up:
			if(sel)
				--sel;
			if((int)(sel - offset) < 0)
				--offset;
			break;
		case down:
			if(sel < items.size() - 1)
				++sel;
			if(sel - offset > 8)
				++offset;
			break;
		case left:
			if(opt)
				--opt;
			break;
		case right:
			if(opt < 2)
				++opt;
			break;
		case enter:
			switch(opt)
			{
			case 0:
				// use
				break;

			case 1:
				// toss
				break;

			case 2:
				// select and move
				break;

			case 3:
				// sort
				break;
			}
		case 'e':
			esc  = true;
		}

		if(esc)
			break;
	}

	// Clear it up
	attron(COLOR_PAIR(text));
	for(int y = _up; y < _up + 21; y++)
	{
		for(int x = _left; x < _left + 52; x++)
		{
			mvprintw(y, x, " ");
		}
	}

	// Render the screen to the state before opening equipment
	entities();
	render();
	ui();
}


int getUserInput()
{
	// Return values:
	// 0 - quit
	// 1 - move or attack
	// 2 - equipment
	// 3 - skip frame (nothing happens)

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
	case eq:
		return 2;
	default:
		map[rogue.y][rogue.x].occupied = true;
		return 3;
	}
	if(map[rogue.x + dx][rogue.y + dy].occupied)
	{
		//attack
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
		return 3;
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

void start_menu()
{
	printw("Welcome to the caves of your doom\n");
	printw("Press any key to start\n");
	printw("Q to quit anytime\n");
	getch();
	clear();
}

int main()
{	
	wininit();
	srand(time(NULL));

	graphics_init();

	start_menu();
	
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

	Item* we = new Item("Dagger", weapon, 5);
	Item* ar = new Item("Worn Chestplate", armor, 3);
	Item* ac = new Item("Old feather", special, 0);

	Item* p1 = new Item("Lesser Potion", consumable, less_heal);
	Item* p2 = new Item("Greater Potion", consumable, great_heal);
	Item* p3 = new Item("Strength Potion", consumable, strength);
	Item* p4 = new Item("Invisibility Potion", consumable, invis);
	Item* p5 = new Item("Weakness Potion", consumable, weak);
	Item* p6 = new Item("Poison Potion", consumable, poison);

	Item* s1 = new Item("Rat tooth", special, tooth);
	Item* s2 = new Item("Witch stone", special, stone);
	Item* s3 = new Item("Harpy feather", special, feather);
	Item* s4 = new Item("Troll tallow", special, tallow);
	Item* s5 = new Item("Spider eye", special, eye);

	items.push_back(*p1);
	items.push_back(*p2);
	items.push_back(*p3);
	items.push_back(*p4);
	items.push_back(*p5);
	items.push_back(*p6);

	items.push_back(*s1);
	items.push_back(*s2);
	items.push_back(*s3);
	items.push_back(*s4);
	items.push_back(*s5);

	rogue.wep_eq = *we;
	rogue.arm_eq = *ar;
	//rogue.spc_eq = *ac;
	delete we;
	delete ar;
	delete ac;
	delete p1;
	delete p2;
	delete p3;
	delete p4;
	delete p5;
	delete p6;
	delete s1;
	delete s2;
	delete s3;
	delete s4;
	delete s5;

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

	Mob test_mob4("rat");
	test_mob4.setSpawn(35, 35);
	map[35][35].seen = true;
	mob_list.push_back(test_mob4);
	
	// Initial render
	entities();
	render();
	ui();

	while(true)
	{
		// TODO
		// Pause menu
		// Actually why would you pause a turn based game with no time events?
		// Equipment menu
		input = getUserInput();
		if(!input)
		{
			message("You stabbed yourself in the face for " + std::to_string(rogue.maxhealth)+ " hp.");
			rogue.health = 0;
			//break;
		}
		else if(input == 2)
		{
			equipment();
			continue;
		}
		else if(input == 3)
		{
			continue;
		}

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

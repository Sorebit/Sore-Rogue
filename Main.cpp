// Sorbet - 2015
//
// Ideas:
//
// COLORFUL POTIONS:
// potions have fixed random colors set on start of the game
// e.x. green - strength, black - poison,
// player doesn't know their effect untill used
//
// ARMOR AND WEAPONS:
// player can't equip them (or can but with downsides) when their strength
// is less than required from a weapon or armor
//
// BETTER SUMMARIES:
// each item should have a short summary
//
// STATUSES:
// some mobs or items can give player a status in which he has to obey special conditions
// for each status there would be a bar in the ui, below health
// it would turn dark from right to left
// e.x. poisoned - lasts 50 steps; player loses 5 - 10 HP every 10 steps
//				   could be used by some bosses or medium mobs
// e.x. invisibility - lasts 25 steps; monsters lose track of the player untill he appears
//					   could be used to escape from tight cluthces etc.
// e.x. confused - lasts 25 steps; player has higher miss chance and sometimes
//                 doesn't follow the input directions
// e.x. weakness - lasts 35 steps; player has lowered defense

// BUGS:
// If you equip an item which has a quantity of more than 1 you equip
// all of its quantity so:
// TODO: unstack 1 item when equipped and stack back, when unequipped
// 

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

const int inf = 1 << 30;

std::vector <char> tileset[] = {{'?'}, {'.'}, {'"'}, {':'}, {'.'}, {'='}, {'#'}, {'~', '-'}, 
								{'.'}, {'@'}, {'&'}, {'o'}, {'+', '.'}, {'1'}, {'\\'}}; 

int cy, cx, input, maxy, maxx, comp_count = 1, max_lake, max_lake_size, comp[300][300], count[300], as = 0;
unsigned sel = 0, offset = 0;

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
		case stairs:
			clear();
			messages_old[0] = "";
			messages_old[1] = "";
			rogue.depth++;
			generate_dungeon(map, rogue);
			break;
	}
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

void statuses()
{
	for(int st = 0; st < 4; ++st)
	{
		if(rogue.status[st] > 0)
			--rogue.status[st];
		switch(st)
		{
			case poisoned:
				if(rogue.status[poisoned] && rogue.status[poisoned] % 10 == 0)
				{
					char damage = rand() % 3 + 1;
					rogue.health -= damage;
					message("You took " + std::to_string((int)damage) + " HP from being poisoned.");
				}
				break;
			case weak:
				// less defense
				break;
			case confused:
				// not always go where the player wants
				break;
			case invisible:
				// should do something in mobs()
				break;
		}
	}
}

void init_player()
{
	// Hard-coded player
	rogue.depth = 1;
	rogue.health = 13;
	rogue.maxhealth = 100;
	rogue.nutr = 10;
	rogue.maxnutr = 10;
	rogue.level = 1;
	rogue.exp = 0;
	rogue.strength = 6;
	rogue.nlvl = 24;
	
	rogue.attack = rogue.strength/2;
	rogue.defense = rogue.strength/2;
}

void start_menu()
{
	printw("Welcome to the caves of your doom\n");
	printw("Arrow keys to move, E to open equipment, Enter or W to select, Q to die\n");
	printw("Press any key to start\n");
	getch();
	clear();
}

int main()
{	
	srand(time(NULL));

	wininit();
	graphics_init();

	start_menu();

	init_player();

	Item* w1 = new Item(weapon, dagger);
	Item* w2 = new Item(weapon, copper);
	Item* w3 = new Item(weapon, iron); 
	Item* w4 = new Item(weapon, steel);
	Item* w5 = new Item(weapon, runic);
	Item* w6 = new Item(weapon, orc);

	Item* a1 = new Item(armor, worn);
	a1->setQuantity(10);
	Item* a2 = new Item(armor, copper);
	Item* a3 = new Item(armor, iron);
	Item* a4 = new Item(armor, steel);
	Item* a5 = new Item(armor, runic);
	Item* a6 = new Item(armor, orc);

	Item* p1 = new Item(consumable, less_heal);
	p1->setQuantity(3);
	Item* p2 = new Item(consumable, great_heal);
	Item* p3 = new Item(consumable, strength);
	p3->setQuantity(5);
	Item* p4 = new Item(consumable, invis);
	Item* p5 = new Item(consumable, weakness);
	Item* p6 = new Item(consumable, poison);
	p6->setQuantity(2);

	Item* s1 = new Item(special, tooth);
	Item* s2 = new Item(special, rune);
	Item* s3 = new Item(special, feather);
	Item* s4 = new Item(special, tallow);
	Item* s5 = new Item(special, eye);
	Item* s6 = new Item(special, ball);

	items.push_back(*w1);
	items.push_back(*w2);
	items.push_back(*w3);
	items.push_back(*w4);
	items.push_back(*w5);
	items.push_back(*w6);

	items.push_back(*a1);
	items.push_back(*a2);
	items.push_back(*a3);
	items.push_back(*a4);
	items.push_back(*a5);
	items.push_back(*a6);

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
	items.push_back(*s6);

	items.push_back(*w1);
	items.push_back(*a1);
	items.push_back(*p1);
	items.push_back(*p2);
	items.push_back(*s1);

	//rogue.wep_eq = *w1;
	//rogue.arm_eq = *a1;
	//rogue.spc_eq = *s2;
	
	delete w1;
	delete w2;
	delete w3;
	delete w4;
	delete w5;
	delete w6;

	delete a1;
	delete a2;
	delete a3;
	delete a4;
	delete a5;
	delete a6;
	
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
	delete s6;

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
		input = getUserInput();
		if(!input)
		{
			message("You stabbed yourself in the face for " + std::to_string(rogue.health)+ " hp.");
			rogue.health = 0;
			//break;
		}
		else if(input == 2)
		{
			// Show equipment
			equipment();
			//Render the screen to the state before opening equipment
			entities();
			render();
			ui();
			continue;
		}
		else if(input == 3)
		{
			continue;
		}

		mobs();
		statuses();
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

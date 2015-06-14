#pragma once

#include <queue>
#include <vector>
#include <stack>
#include <algorithm>
#include <string>
#include <ncurses.h>
#include <algorithm>
#include <random>
#include <chrono>


#include "Globals.h"

class Mob
{
	private:
		std::string name;
		char tile;
		int x, y;
		int health, maxhealth;
		int attack_rate;
		
		
	public:
		int left_to_step;
		int walk_rate;
		std::stack < std::pair <int, int> > path_to_player;

		// Get attributes
		char getTile();
		std::string getName();
		std::pair <int, int> getHealth();
		std::pair <int, int> getPos();

		// Actions
		void setSpawn(int py, int px);
		bool seesPlayer(Character rogue);
		void findPath(int py, int px);
		std::pair <int, int> getNextStep();
		void walk(std::pair <int, int> );
		void clearPath();

		Mob(Character rogue, std::string name, int maxheal);
};

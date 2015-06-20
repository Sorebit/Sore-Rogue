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
		int walk_rate;
		int follow_time; // -1 means mob is passive and wont attack or follow the player
		std::stack < std::pair <int, int> > path_to_player;
		
	public:
		int attack_counter;
		int walk_counter;
		int follow_timeout;

		// Get attributes
		int getAttackRate();
		int getWalkRate();
		int getFollowTime();
		int distFrom(int qy, int qx);
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

		Mob(std::string spieces);
};

extern std::vector <Mob> mob_list;
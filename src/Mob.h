// Sorbet - 2015
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
		int luck;
		int strength, armor;
		std::stack < std::pair <int, int> > path_to_player;
		std::vector <std::string> verbs;
		
	public:
		int walk_counter;
		int follow_timeout;

		// Get attributes
		int getWalkRate();
		int getFollowTime();
		int getLuck();
		int getStrength();
		int getArmor();
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
		int attack();
		int distFrom(int qy, int qx);
		std::string getVerb();

		Mob(std::string spieces);
};

extern std::vector <Mob> mob_list;
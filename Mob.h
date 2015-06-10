#ifndef MOB_H
#define MOB_H

#include <queue>
#include <vector>
#include <algorithm>
#include <string>

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
		std::queue < std::pair <int, int> > path;

		// Get attributes
		char getTile();
		std::string getName();
		std::pair <int, int> getHealth();
		std::pair <int, int> getPos();

		// Actions
		void walk(std::pair <int, int> );
		void setSpawn(int py, int px);
		bool seesPlayer(Character rogue);
		void findPath(int py, int px);

		Mob(Character rogue, std::string name, int maxheal);
};

#endif
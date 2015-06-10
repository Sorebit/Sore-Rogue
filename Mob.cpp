#include "Mob.h"

Mob::Mob(Character rogue, std::string sName, int maxheal)
{
	// Set a non 0 name and make the tile its first letter
	name = (sName != "") ? sName : "null";
	name[0] = toupper(name[0]);
	tile = tolower(name[0]);
	maxhealth = maxheal;
	health = maxhealth - 2;
}

// Get attributes
std::pair <int, int> Mob::getPos() { return {y, x}; }

char Mob::getTile() { return tile; }

std::string Mob::getName() { return name; }

std::pair <int, int> Mob::getHealth() { return {health, maxhealth}; }

// Actions
void Mob::setSpawn(int py, int px)
{
	y = py;
	x = px;
	map[y][x].occupied = true;
}

void Mob::walk(std::pair <int, int> step)
{
	Tile checked = map[y + step.first][x + step.second];
	if(checked.tile == zero || checked.tile == wall || checked.tile == pit)
		return;
	if(checked.tile == water || checked.tile == door || checked.occupied)
		return;
	map[y][x].occupied = false;
	y += step.first;
	x += step.second;
	map[y][x].occupied = true;

}

bool Mob::seesPlayer(Character rogue)
{
	// Check if a mob can see the player (actually the oposite but who cares)
	// Probably should use drawing lines
	return map[y][x].inView;
}

void Mob::findPath(int y, int x)
{
	return;
}
// Sorbet - 2015
/* IDEAS:
	Colorful potions:
	potions have fixed random colors set on start of the game
	e.x. green - strength, black - poison,
	player doesn't know their effect untill used

	Use of weapons and armor:
	player can't equip them (or can but with downsides) when their strength
	is less than required from a weapon or armor
*/
#pragma once

#include <string>

enum types
{
	consumable,
	weapon,
	armor,
	special,
};

enum consumables
{
	less_heal,
	great_heal,
	strength,
	invis,
	weak,
	poison,
};

enum artifacts
{
	// Ideas
	tooth, // attack+
	stone, // armor+
	feather, // speed+ 
	tallow, // knockback
	eye, // greater view 
};

class Item
{
	private:
	std::string name;
	int type;
	int value;

	public:
	int getType();
	int getValue();
	std::string getName();	
	std::string getInfo();
	void use();

	Item(std::string _name, int _type, int _value);
	Item();
	~Item();
};
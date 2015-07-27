// Sorbet - 2015
#include "Item.h"

Item::Item() 
{
	name = "none";
	type = 0;
	value = 0;
}

Item::Item(std::string _name, int _type, int _value)
{
	name = _name;
	type = _type;
	value = _value;
}

Item::~Item()
{

}

int Item::getType() { return type; }
int Item::getValue() { return value; }
std::string Item::getName() { return name; }

std::string Item::getInfo()
{
	std::string info;
	switch(type)
	{
		case consumable:
		{
			info += "Consumable: ";
			switch(value)
			{
				case less_heal:
					info += "Heals 25 HP";
					break;
				case great_heal:
					info += "Heals 50 HP";
					break;
				case strength:
					info += "Increases strength";
					break;
				case invis:
					info += "Invisibility";
					break;
				case weak:
					info += "Weakness";
					break;
				case poison:
					info += "Poison";
					break;
			}
			break;
		} // consumable

		case weapon:
		{
			info += "Weapon: ";
			info += "Attack " + std::to_string(value);
			break;
		} 

		case armor:
		{
			info += "Armor: ";
			info += "Defense " + std::to_string(value);
			break;
		}

		case special:
		{
			info += "Artifact: ";
			switch(value)
			{
				case tooth: 
					info += "Increased attack";
					break;
				case stone: 
					info += "Increased armor";
					break;
				case feather: 
					info += "Birds don't attack you";
					break; 
				case tallow: 
					info += "Knockback on hit";
					break;
				case eye: 
					info += "Increased field of view";
					break; 
			}
			break;
		}
	}

	return info;

}
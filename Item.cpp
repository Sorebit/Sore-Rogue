// Sorbet - 2015
#include "Item.h"

Item::Item() 
{
	name = "none";
	type = 0;
	id = 0;
}

Item::Item(int _type, int _id)
{
	type = _type;
	id = _id;

	switch(_type)
	{
		case consumable:
		{
			pickup_message = "It's just a hazy fluid in a bottle.";
			switch(_id)
			{
				case less_heal:
					name = "Lesser Potion";
					summary = "Heals 25 HP";
					break;
				case great_heal:
					name = "Greater Potion";
					summary = "Heals 50 HP";
					break;
				case strength:
					name = "Strength Potion";
					summary = "Increases strength";
					break;
				case invis:
					name = "Invisibility Potion";
					summary = "Grants temporary invisibility";
					break;
				case weak:
					name = "Weakness Potion";
					summary = "Weakness";
					break;
				case poison:
					name = "Poison Potion";
					summary = "Poison";
					break;
			}
			break;
		}
		case weapon:
		{
			switch(_id)
			{
				case dagger:
					name = "Copper Dagger";
					pickup_message = "You could cut some bread with this.";
					value = 5;
					break;
				case axe:
					name = "Wood axe";
					pickup_message = "Chopping wood is neat.";
					value = 8;
					break;
				case sword:
					name = "Iron Sword";
					pickup_message = "That's a fine piece of metal.";
					value = 12;
					break;
				case hammer:
					name = "Orc Hammer";
					pickup_message = "You suddenly find myself in need for smashing.";
					value = 17;
					break;
				case spear:
					name = "Amazonian Spear";
					pickup_message = "Who's up for some troll kebab?";
					value = 24;
					break;
				case scepter:
					// Damage in all 4 direcrions
					name = "Magical Scepter";
					summary = "Attack 8 (Creates a field of dark energy)";
					pickup_message = "You can feel a dark presence crawling into your backpack.";
					value = 8;
					return;	
			}
			summary = "Attack " + std::to_string(value);
			break;
		}
		case armor:
		{
			switch(_id)
			{
				case worn:
					name = "Worn shirt";
					pickup_message = "You shouldn't put that on yourself.";
					value = 2;
					break;
				case copper:
					name = "Copper Chainmail";
					pickup_message = "It has holes in it. Should it?";
					value = 7;
					break;
				case iron:
					name = "Iron Chestplate";
					pickup_message = "It looks somewhat solid.";
					value = 12;
					break;
				case steel:
					name = "Steel Armor";
					pickup_message = "Seems like a decent thing to put on oneself.";
					value = 18;
					break;
				case runic:
					// Magic defense
					name = "Runic Armor";
					summary = "Defense 21 (Grants partial magic immunity)";
					pickup_message = "It ooks magical.";
					value = 20;
					return;
				case orc:
					// High armor
					name = "Orc Armor";
					pickup_message = "Orcs must've been killing each other for this scrap.";
					value = 25;
					break;
			}
			summary += "Defense " + std::to_string(value);
			break;
		}
		case special:
		{
			switch(_id)
			{
				case tooth:
					name = "Rat tooth";
					summary = "Increased attack";
					pickup_message = "Did you hit it THAT hard?";
					break;
				case rune:
					name = "Moon rune";
					summary = "Increased armor";
					pickup_message = "It glows with a blue light.";
					break;
				case feather:
					name = "Harpy feather";
					summary = "Birds don't attack you";
					pickup_message = "Mom would like it.";
					break; 
				case tallow:
					name = "Troll tallow";
					summary = "Knockback on hit";
					pickup_message = "It's sticky.";
					break;
				case eye: 
					name = "Spider eye";
					summary = "Increased field of view";
					pickup_message = "It looks at you with repugnance.";
					break;
				case ball:
					name = "Ball of moss";
					pickup_message = "It has a pleasant texture.";
					summary = "No idea what effect";
					break;
			}
			break;
		}
	}
}

Item::Item(std::string _name, int _type, int _id)
{
	name = _name;
	type = _type;
	id = _id;
}

Item::~Item()
{

}

int Item::getType() { return type; }
int Item::getId() { return id; }
std::string Item::getName() { return name; }
std::string Item::getMessage() { return pickup_message; }

std::string Item::getInfo()
{
	std::string info;
	switch(type)
	{
		case consumable:
			info = "Consumable: ";
			break;
		case weapon:
			info = "Weapon: ";
			break;
		case armor:
			info = "Armor: ";
			break;
		case special:
			info = "Artifact: ";
			break;
	}

	return info + summary;
}
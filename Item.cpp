// Sorbet - 2015

#include "Item.h"
#include "Graphics.h"

Item::Item() 
{
	name = "Not equipped";
}

Item::Item(std::string _name, int _type, int _id)
{
	name = _name;
	type = _type;
	id = _id;
}

Item::Item(int _type, int _id)
{
	type = _type;
	id = _id;

	switch(_type)
	{
		case consumable:
		{
			quantity = 1;
			summary = "Effect: ";
			pickup_message = "It's just a hazy fluid in a bottle.";
			switch(_id)
			{
				case less_heal:
					name = "Lesser Potion";
					summary += "Heals 25 HP";
					break;
				case great_heal:
					name = "Greater Potion";
					summary += "Heals 50 HP";
					break;
				case strength:
					name = "Strength Potion";
					summary += "Increases strength";
					break;
				case invis:
					name = "Invisibility Potion";
					summary += "Grants temporary invisibility";
					break;
				case weakness:
					name = "Weakness Potion";
					summary += "Weakness";
					break;
				case poison:
					name = "Poison Potion";
					summary += "Poison";
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
					requires = 3;
					break;
				case axe:
					name = "Wooden axe";
					pickup_message = "Chopping wood is neat.";
					value = 8;
					requires = 6;
					break;
				case sword:
					name = "Iron Sword";
					pickup_message = "It's a fine piece of metal.";
					value = 12;
					requires = 9;
					break;
				case hammer:
					name = "Orc Hammer";
					pickup_message = "You suddenly find myself in need for smashing.";
					value = 17;
					requires = 12;
					break;
				case spear:
					name = "Amazonian Spear";
					pickup_message = "Who's up for some troll kebab?";
					value = 24;
					requires = 15;
					break;
				case scepter:
					// Damage in all 4 direcrions
					name = "Magical Scepter";
					summary = "Attack 8 (Creates a field of dark energy)";
					pickup_message = "You can feel a dark presence crawling into your backpack.";
					value = 8;
					requires = 15;
					return;	
			}
			value += rand() % 3 - 1;
			summary = "Weapon: Grants " + std::to_string(value) + " attack";
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
					requires = 3;
					break;
				case copper:
					name = "Copper Chainmail";
					pickup_message = "It has holes in it. Should it?";
					value = 7;
					requires = 6;
					break;
				case iron:
					name = "Iron Chestplate";
					pickup_message = "It looks somewhat solid.";
					value = 12;
					requires = 9;
					break;
				case steel:
					name = "Steel Armor";
					pickup_message = "Seems like a decent thing to put on oneself.";
					value = 18;
					requires = 12;
					break;
				case runic:
					// Magic defense
					name = "Runic Armor";
					summary = "Defense 21 (Grants partial magic immunity)";
					pickup_message = "It ooks magical.";
					value = 20;
					requires = 15;
					return;
				case orc:
					// High armor
					name = "Orc Armor";
					pickup_message = "Orcs must've been killing each other for this scrap.";
					value = 25;
					requires = 15;
					break;
			}
			value += rand() % 3 - 1;
			summary += "Armor: Grants " + std::to_string(value) + " defense";
			break;
		}
		case special:
		{

			summary = "Artifact: ";
			switch(_id)
			{
				case tooth:
					name = "Rat tooth";
					summary += "Increased attack";
					pickup_message = "Did you hit it THAT hard?";
					break;
				case rune:
					name = "Moon rune";
					summary += "Increased armor";
					pickup_message = "It glows with a blue light.";
					break;
				case feather:
					name = "Harpy feather";
					summary += "Birds don't attack you";
					pickup_message = "Mom would like it.";
					break; 
				case tallow:
					name = "Troll tallow";
					summary += "Knockback on hit";
					pickup_message = "It's sticky.";
					break;
				case eye: 
					name = "Spider eye";
					summary += "Increased field of view";
					pickup_message = "It looks at you with repugnance.";
					break;
				case ball:
					name = "Ball of moss";
					pickup_message = "It has a pleasant texture.";
					summary += "Increased health";
					break;
			}
			break;
		}
	}
}


Item::~Item()
{

}

int Item::getType() { return type; }
int Item::getId() { return id; }
int Item::getValue() { return value; }
int Item::getRequire() { return requires; }
int Item::getQuantity() { return quantity; }
std::string Item::getName() { return name; }
std::string Item::getMessage() { return pickup_message; }
std::string Item::getSummary() { return summary; }

void Item::setType(int _type) { type = _type; }
void Item::setId(int _id) { id = _id; }
void Item::setValue(int _value) { value = _value; }
void Item::setQuantity(int _quantity) { quantity = _quantity; }
void Item::setName(std::string _name) { name = _name; }
void Item::setSummary(std::string _summary) { summary = _summary; }
void Item::setMessage(std::string _pickup_message) { pickup_message = _pickup_message; }

void Item::swap(Item &_item)
{
	std::swap(_item, *this);
}

bool Item::use()
{
	switch(type)
	{
		case consumable:
			switch(id)
			{
				case less_heal:
					rogue.health += 25;
					rogue.health = (rogue.health % rogue.maxhealth) ? rogue.health : rogue.maxhealth;
					message("You feel a little bit better.");
					break;
				case great_heal:
					rogue.health += 50;
					rogue.health = (rogue.health < rogue.maxhealth) ? rogue.health : rogue.maxhealth;
					message("You feel revitalized.");
					break;
				case strength:
					rogue.strength++;
					rogue.status[confused] = 25;
					message("Your head gets light. You feel dizzy.");
					break;
				case invis:
					rogue.status[invisible] = 25;
					message("You can't see your hands anymore.");
					render_player();
					break;
				case weakness:
					rogue.status[weak] = 35;
					message("You feel weak as a shrimp.");
					break;
				case poison:
					rogue.status[poisoned] = 50;
					rogue.health -= 4;
					message("You can feel the poison eating you from inside.");
					break;
			}
			ui();
			message("befor erase");
			erase_current_item();
			message("after erase");
			break;

		case weapon:
			if(this->getRequire() > rogue.strength)
			{
				message("You need " + std::to_string( this->getRequire() ) + " strength to equip this.");
				return false;
			}
			rogue.attack = rogue.strength/2 + this->getValue();
			message("You took " + name + " out.");
			std::swap(rogue.wep_eq, *this);
			break;

		case armor:
			if(this->getRequire() > rogue.strength)
			{
				message("You need " + std::to_string( this->getRequire() ) + " strength to equip this.");
				return false;
			}
			rogue.defense = rogue.strength/2 + this->getValue();
			message("You put " + name + " on.");
			this->swap(rogue.arm_eq);
			break;

		case special:
			message("You used " + name + ".");
			this->swap(rogue.spc_eq);
			break;
	}
	
	return true;
}

bool Item::toss()
{
	message("You tossed " + name + " out");
	return true;
}
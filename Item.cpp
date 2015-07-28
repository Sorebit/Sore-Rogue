// Sorbet - 2015

#include "Item.h"
#include "Graphics.h"

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
			summary = "Consumable: ";
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
				case weak:
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
					break;
				case axe:
					name = "Wood axe";
					pickup_message = "Chopping wood is neat.";
					value = 8;
					break;
				case sword:
					name = "Iron Sword";
					pickup_message = "It's a fine piece of metal.";
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
			summary = "Weapon: Attack " + std::to_string(value);
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
			summary += "Armor: Defense " + std::to_string(value);
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
					summary += "No idea what effect";
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

Item::Item(int _type, int _id, int _value, std::string _name, std::string _summary, std::string _message)
{
	type = _type;
	id = _id;
	value = _value;
	name = _name;
	summary = _summary; 
	pickup_message = _message;
}

Item::~Item()
{

}

int Item::getType() { return type; }
int Item::getId() { return id; }
int Item::getValue() { return value; }
std::string Item::getName() { return name; }
std::string Item::getMessage() { return pickup_message; }
std::string Item::getSummary() { return summary; }

void Item::setType(int _type) { type = _type; }
void Item::setId(int _id) { id = _id; }
void Item::setValue(int _value) { value = _value; }
void Item::setName(std::string _name) { name = _name; }
void Item::setSummary(std::string _summary) { summary = _summary; }
void Item::setMessage(std::string _pickup_message) { pickup_message = _pickup_message; }

void Item::swap(Item _item)
{
	Item* temp = new Item(type, id, value, name, summary, pickup_message);

//	this->setType(_item.getType()); 
//	this->setId(_item.getId());
//	this->setValue(_item.getValue()); 
//	this->setName(_item.getName()); 
//	this->setSummary(_item.getSummary()); 
//	this->setMessage(_item.getMessage());

//	_item.setType(temp->getType());
//	_item.setId(temp->getId());
//	_item.setValue(temp->getValue());
//	_item.setName(temp->getName());
//	_item.setSummary(temp->getSummary());
//	_item.setMessage(temp->eqegetMessage());
	std::swap(_item, *this);
	delete temp;
}

bool Item::use()
{
	switch(type)
	{
		case consumable:
			
			break;
		case weapon:
			this->swap(rogue.wep_eq);
			break;
		case armor:
			this->swap(rogue.arm_eq);
			break;
		case special:
			this->swap(rogue.spc_eq);
			break;
	}
	message("Used " + name);
	return true;
}

bool Item::toss()
{
	message("Tossed " + name);
	return true;
}
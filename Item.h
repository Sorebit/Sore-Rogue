// Sorbet - 2015
//
// Ideas:
//
// COLORFUL POTIONS:
// potions have fixed random colors set on start of the game
// e.x. green - strength, black - poison,
// player doesn't know their effect untill used
//
// ARMOR AND WEAPONS:
// player can't equip them (or can but with downsides) when their strength
// is less than required from a weapon or armor
//
// BETTER SUMMARIES:
// each item should have a short summary

#pragma once

#include <string>

enum types { consumable, weapon, armor, special, };

enum consumables { less_heal, great_heal, strength, invis, weak, poison, };

enum weapons { dagger, axe, sword, hammer, spear, scepter, };

enum armors { worn, copper, iron, steel, runic, orc, };

enum artifacts { tooth, rune, feather, tallow, eye, ball, };

class Item
{
	private:
	std::string name, summary, pickup_message;
	int type, value, id;

	public:
	int getType();
	int getId();
	int getValue();
	std::string getName();	
	std::string getSummary();
	std::string getMessage();

	void setType(int _type);
	void setId(int _id);
	void setValue(int _value);
	void setName(std::string _name);
	void setSummary(std::string _summary);
	void setMessage(std::string _pickup_message);

	void swap(Item _item);
	bool use();
	bool toss();

	Item();
	Item(int _type, int _id);
	Item(std::string _name, int _type, int _value);
	Item(int _type, int _id, int _value, std::string _name, std::string _summary, std::string _message);
	~Item();
};
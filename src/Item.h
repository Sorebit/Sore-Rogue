// Sorbet - 2015
//
// items other than consumables should never stack, 
// keep this in mind when writing pickup code

#pragma once

#include <string>

enum types { consumable, weapon, armor, special, };

enum consumables { less_heal, great_heal, strength, invis, weakness, poison, };

enum weapons { dagger, axe, sword, hammer, spear, scepter, };

enum armors { worn, copper, iron, steel, runic, orc, };

enum artifacts { tooth, rune, feather, tallow, eye, ball, };

class Item
{
	private:
	std::string name, summary, pickup_message;
	int type, value, id, requires, quantity;

	public:
	int getType();
	int getId();
	int getValue();
	int getRequire();
	int getQuantity();
	std::string getName();	
	std::string getSummary();
	std::string getMessage();

	void setType(int _type);
	void setId(int _id);
	void setValue(int _value);
	void setQuantity(int _quantity); 
	void setName(std::string _name);
	void setSummary(std::string _summary);
	void setMessage(std::string _pickup_message);

	void swap(Item &_item);
	bool use();
	bool toss();

	Item();
	Item(int _type, int _id);
	Item(std::string _name, int _type, int _value);
	Item(int _type, int _id, int _value, std::string _name, std::string _summary, std::string _message);
	~Item();

};

bool compare_items(Item item1, Item item2);
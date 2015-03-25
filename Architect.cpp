#include "Architect.h"

// Actually not architect at the moment
// Hard-coded test levels

void rect(Tile tmap[][200], int y, int x, int h, int w, int t1, int t2, bool corners, int t3)
{
	for(int i = y; i < y + h; i++)
	{
		for(int j = x; j < x + w; j++)
		{
			if(i == y || i == y + h - 1 || j == x || j == x + w - 1)
				tmap[i][j].tile = t2;
			else
				tmap[i][j].tile = t1;
		}
	}
	if(!corners)
		tmap[y][x].tile = tmap[y][x + w - 1].tile = tmap[y + h - 1][x].tile = tmap[y + h - 1][x + w - 1].tile = t3;
}

void lighting_test(Tile tmap[][200], Character & crogue)
{
	crogue.y = 21;
	crogue.x = 43;
	rect(tmap, 10, 20, 23, 47, path, wall);
	rect(tmap, 13, 30, 3, 6, zero, wall);
	rect(tmap, 13, 51, 3, 6, zero, wall);
	rect(tmap, 20, 30, 3, 6, zero, wall);
	rect(tmap, 20, 51, 3, 6, zero, wall);
	rect(tmap, 27, 30, 3, 6, zero, wall);
	rect(tmap, 27, 51, 3, 6, zero, wall);
	return;
}

void test_map(Tile tmap[][200], Character & crogue)
{
	crogue.y = 21;
	crogue.x = 48;
	crogue.keys = 0;
	crogue.level = 6;
	crogue.exp = 65;
	crogue.nlvl = 100;
	crogue.strength = 13;
	crogue.armor = 8;
	crogue.maxhealth = 10;
	crogue.health = 5; 
	crogue.nutr = 6;
	crogue.maxnutr = 15;
	crogue.gold = 657;
	crogue.depth = 5;

	//komora testowa 1
	rect(tmap, 8, 10, 15, 30, path, wall);

	//komora testowa 2
	rect(tmap, 8, 45, 15, 30, path, wall);
	//komora testowa 3
	rect(tmap, 25, 45, 15, 30, path, wall);
	rect(tmap, 28, 46, 11, 23, edge, edge);
	rect(tmap, 29, 46, 10, 22, pit, pit);
	rect(tmap, 28, 51, 4, 1, bridge, bridge);
	rect(tmap, 28, 55, 4, 1, bridge, bridge);
	rect(tmap, 31, 51, 1, 5, bridge, bridge);
	rect(tmap, 31, 53, 4, 1, bridge, bridge);
	rect(tmap, 35, 60, 3, 1, bridge, bridge);
	rect(tmap, 35, 53, 1, 16, bridge, bridge);
	rect(tmap, 37, 61, 1, 8, bridge, bridge);
	//komora testowa 4
	rect(tmap, 25, 80, 15, 30, path, wall);

	//przejscie 1 do 2
	rect(tmap, 13, 39, 5, 7, path, wall);
	rect(tmap, 14, 39, 3, 1, path, path);

	//przejscie 2 do 3
	rect(tmap, 22, 55, 4, 10, path, wall);
	rect(tmap, 22, 56, 1, 8, path, path);
	rect(tmap, 25, 56, 1, 8, path, path);

	//przejscie z 3 do 4
	rect(tmap, 30, 74, 5, 7, path, wall);

	//staw
	rect(tmap, 9, 12, 7, 12, coast, coast, false, path);
	rect(tmap, 10, 13, 5, 10, water, water, false, coast);
	rect(tmap, 10, 59, 7, 14, grass, grass, false, path);
	tmap[20][33].tile = tmap[20][34].tile = tmap[19][33].tile = tmap[20][32].tile = gold;
	tmap[15][60].tile = tmap[14][59].tile = tmap[12][68].tile = tmap[16][65].tile = grass;

	//klucze i drzwi
	tmap[19][28].tile = tmap[32][77].tile = tmap[37][70].tile = key;
	tmap[32][74].tile = tmap[15][45].tile = tmap[32][80].tile = door;
}
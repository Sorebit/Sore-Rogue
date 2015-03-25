//TODO:
//Zmniejszyć ilość kolorów
//Losowe plansze
//Moby
//Walka z mobami
//Itemsy

#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>
#include <string>
#include <cmath>

struct Character
{
	int x, y, depth; 
	int health, maxhealth; 
	int strength, armor; 
	int exp, level, nlvl; 
	int nutr, maxnutr;
	int gold, keys; 
} rogue;

struct Tile
{ 
	int door_open, tile; 
	bool seen, inView; 
} map[200][200];

enum Tiles 
{
	//In view
	zero = 0,
	path = 1,
	grass = 2,
	pit = 3,
	edge = 4,
	bridge = 5,
	wall = 6,
	water = 7,
	coast = 8,
	player = 9,
	mob = 10,
	gold = 11,
	door = 12,
	key = 13,
	text = 14,
	//Out of view
	opath = 21,
	ograss = 22,
	opit = 23,
	oedge = 24,
	obridge = 25,
	owall = 26,
	owater = 27,
	ocoast = 28,
	ogold = 31,
	odoor = 32,
	okey = 33,
	//UI
	ui1 = 50,
	bui1 = 51,
	ui2 = 53,
	bui2 = 54,
	uitext = 55,
};

int map_mx, map_maxy;
std::vector <char> tileset[] = {{'?'}, {'.'}, {'"'}, {':'}, {'.'}, {'='}, {'#'}, {'~', '-'}, {'.'}, {'@'}, {'&'}, {'o'}, {'+', '.'}, {'1'}}; 

struct Disp { int h, w; } display;

enum keyboard
{
	down = 258,
	up = 259,
	left = 260,
	right = 261,
	quit = 113,
};

enum Colors 
{
	//In view
	COLOR_DARK_BLUE = 17,
	COLOR_GOLD = 18,
	COLOR_LIGHT_GREEN = 19,
	COLOR_WATER = 20,
	COLOR_LIGHT_WATER = 21,
	COLOR_BROWN = 22,
	COLOR_LIGHT_BROWN = 23,
	COLOR_GREY = 24,
	//Out of view
	COLOR_OOV_GREY = 25,
	COLOR_OOV_DARK_GREY = 26,
	COLOR_OOV_BLUE = 27,
	COLOR_OOV_WATER = 28,
	COLOR_OOV_LIGHT_WATER = 29,
	COLOR_OOV_LIGHT_GREEN = 30,
	COLOR_OOV_GOLD = 31,
	COLOR_OOV_BROWN = 32,
	COLOR_OOV_LIGHT_BROWN = 33,
	//UI
	COLOR_UI1 = 50,
	COLOR_BUI1 = 51,
	COLOR_UI2 = 52,
	COLOR_BUI2 = 53,
};

void graphics_init()
{
	start_color();

	//custom color definitons
	init_color(COLOR_BLUE, 45, 60, 350);
	init_color(COLOR_DARK_BLUE, 0, 0, 80);
	init_color(COLOR_GOLD, 1000, 900, 300);
	init_color(COLOR_LIGHT_GREEN, 615, 1000, 490);
	init_color(COLOR_BROWN, 322, 78, 78);
	init_color(COLOR_LIGHT_BROWN, 784, 560, 374);
	init_color(COLOR_WATER, 500, 930, 1000);
	init_color(COLOR_LIGHT_WATER, 600, 1000, 1000);
	init_color(COLOR_GREY, 700, 700, 700);

	init_color(COLOR_OOV_GREY, 300, 300, 370);
	init_color(COLOR_OOV_DARK_GREY, 150, 150, 220);
	init_color(COLOR_OOV_BLUE, 0, 0, 30);
	init_color(COLOR_OOV_WATER, 0, 290, 350);
	init_color(COLOR_OOV_LIGHT_WATER, 0, 440, 500);
	init_color(COLOR_OOV_LIGHT_GREEN, 215, 400, 90);
	init_color(COLOR_OOV_GOLD, 400, 400, 0);
	init_color(COLOR_OOV_BROWN, 70, 20, 20);
	init_color(COLOR_OOV_LIGHT_BROWN, 284, 150, 150);

	init_color(COLOR_UI1, 141, 188, 710);
	init_color(COLOR_BUI1, 0, 0, 250);
	init_color(COLOR_UI2, 41, 88, 610);
	init_color(COLOR_BUI2, 0, 0, 150);

	//Custom color pairs

	//Path & Walls
	init_pair(path, COLOR_GREY, COLOR_DARK_BLUE);
	init_pair(grass, COLOR_LIGHT_GREEN, COLOR_DARK_BLUE);
	init_pair(pit, COLOR_OOV_GREY, COLOR_BLACK);
	init_pair(edge, COLOR_WHITE, COLOR_BLUE);
	init_pair(bridge, COLOR_LIGHT_BROWN, COLOR_BROWN);
	init_pair(wall, COLOR_BLACK, COLOR_WHITE);
	init_pair(coast, COLOR_WHITE, COLOR_LIGHT_WATER);
	init_pair(door, COLOR_LIGHT_BROWN, COLOR_BROWN);
	init_pair(key, COLOR_GOLD, COLOR_DARK_BLUE);

	init_pair(opath, COLOR_OOV_GREY, COLOR_OOV_BLUE);
	init_pair(ograss, COLOR_OOV_LIGHT_GREEN, COLOR_OOV_BLUE);
	init_pair(opit, COLOR_OOV_DARK_GREY, COLOR_BLACK);
	init_pair(oedge, COLOR_OOV_GREY, COLOR_DARK_BLUE);
	init_pair(obridge, COLOR_OOV_LIGHT_BROWN, COLOR_OOV_BROWN);
	init_pair(owall, COLOR_BLACK, COLOR_OOV_GREY);
	init_pair(ocoast, COLOR_GREY, COLOR_OOV_LIGHT_WATER);
	init_pair(odoor, COLOR_OOV_LIGHT_BROWN, COLOR_OOV_BROWN);
	init_pair(okey, COLOR_OOV_GOLD, COLOR_OOV_BLUE);

	//Mobs
	init_pair(player, COLOR_WHITE, COLOR_DARK_BLUE);
	init_pair(mob, COLOR_GOLD, COLOR_DARK_BLUE);

	//Fluids
	init_pair(water, COLOR_LIGHT_WATER, COLOR_WATER);
	init_pair(owater, COLOR_OOV_LIGHT_WATER, COLOR_OOV_WATER);

	//Other
	init_pair(gold, COLOR_GOLD, COLOR_DARK_BLUE);
	init_pair(ogold, COLOR_OOV_GOLD, COLOR_OOV_BLUE);

	init_pair(text, COLOR_WHITE, COLOR_BLACK);
	init_pair(ui1, COLOR_GREY, COLOR_UI1);
	init_pair(bui1, COLOR_GREY, COLOR_BUI1);
	init_pair(ui2, COLOR_GREY, COLOR_UI2);
	init_pair(bui2, COLOR_GREY, COLOR_BUI2);
	init_pair(uitext, COLOR_GREY, COLOR_BLACK);

	//Cursor and invisible input
	curs_set(0);
	noecho();
}

bool isPath(int x, int y)
{
	if(x < 0 || x > map_mx - 1 || y < 0 || y > display.h - 1)
		return false;
	if(map[y][x].tile == path || map[y][x].tile == grass || map[y][x].tile == gold ||  map[y][x].tile == key || map[y][x].tile == edge || map[y][x].tile == bridge)
		return true;
	if(map[y][x].tile == door && (rogue.keys || map[y][x].door_open) )
		return true;
	return false;
}

int getMovement()
{
	int key = getch();
	switch(key)
	{
	case quit:
		return 0;
		break;
	case up:
		if(isPath(rogue.x, rogue.y - 1))
			rogue.y--;
		break;
	case down:
		if(isPath(rogue.x, rogue.y + 1))
			rogue.y++;
		break;
	case left:
		if(isPath(rogue.x - 1, rogue.y))
			rogue.x--;
		break;
	case right:
		if(isPath(rogue.x + 1, rogue.y))
			rogue.x++;
		break;

	}
	return 1;
}

void rect(int y, int x, int h, int w, int t1, int t2, bool corners = true, int t3 = 0)
	{
		for(int i = y; i < y + h; i++)
		{
			for(int j = x; j < x + w; j++)
			{
				if(i == y || i == y + h - 1 || j == x || j == x + w - 1)
					map[i][j].tile = t2;
				else
					map[i][j].tile = t1;
			}
		}
		if(!corners)
			map[y][x].tile = map[y][x + w - 1].tile = map[y + h - 1][x].tile = map[y + h - 1][x + w - 1].tile = t3;
	}

/*void test_map()
{
	rogue.y = 21;
	rogue.x = 48;
	rogue.keys = 0;
	rogue.level = 6;
	rogue.exp = 65;
	rogue.nlvl = 100;
	rogue.strength = 13;
	rogue.armor = 8;
	rogue.maxhealth = 10;
	rogue.health = 5; 
	rogue.nutr = 6;
	rogue.maxnutr = 15;
	rogue.gold = 657;
	rogue.depth = 5;

	//komora testowa 1
	rect(8, 10, 15, 30, path, wall);

	//komora testowa 2
	rect(8, 45, 15, 30, path, wall);
	//komora testowa 3
	rect(25, 45, 15, 30, path, wall);
	rect(28, 46, 11, 23, edge, edge);
	rect(29, 46, 10, 22, pit, pit);
	rect(28, 51, 4, 1, bridge, bridge);
	rect(28, 55, 4, 1, bridge, bridge);
	rect(31, 51, 1, 5, bridge, bridge);
	rect(31, 53, 4, 1, bridge, bridge);
	rect(35, 60, 3, 1, bridge, bridge);
	rect(35, 53, 1, 16, bridge, bridge);
	rect(37, 61, 1, 8, bridge, bridge);
	//komora testowa 4
	rect(25, 80, 15, 30, path, wall);

	//przejscie 1 do 2
	rect(13, 39, 5, 7, path, wall);
	rect(14, 39, 3, 1, path, path);

	//przejscie 2 do 3
	rect(22, 55, 4, 10, path, wall);
	rect(22, 56, 1, 8, path, path);
	rect(25, 56, 1, 8, path, path);

	//przejscie z 3 do 4
	rect(30, 74, 5, 7, path, wall);

	//staw
	rect(9, 12, 7, 12, coast, coast, false, path);
	rect(10, 13, 5, 10, water, water, false, coast);
	rect(10, 59, 7, 14, grass, grass, false, path);
	map[20][33].tile = map[20][34].tile = map[19][33].tile = map[20][32].tile = gold;
	map[15][60].tile = map[14][59].tile = map[12][68].tile = map[16][65].tile = grass;

	//klucze i drzwi
	map[19][28].tile = map[32][77].tile = map[37][70].tile = key;
	map[32][74].tile = map[15][45].tile = map[32][80].tile = door;
}*/

void yray(int x, int y1, int y2)
{
	int dir = 1 - 2 * (y1 > y2);
	for(int y = y1; y*dir <= y2*dir; y += dir)
	{
		map[y][x].seen = true;
		map[y][x].inView = true;
		if(map[y][x].tile == wall || map[y][x].tile == door)
			break;
	}
}

void xray(int x1, int x2, int y)
{
	int dir = 1 - 2 * (x1 > x2);
	for(int x = x1; x*dir <= x2*dir; x += dir)
	{
		map[y][x].seen = true;
		map[y][x].inView = true;
		if(map[y][x].tile == wall || map[y][x].tile == door)
			break;
	}
}

void dray(float x1, float y1, float x2, float y2)
{
	std::vector < std::pair <int, int> > q;
	bool swx = false;
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if(steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}
	if(x1 > x2)
	{
		swx = true;
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;

	for(int x = (int)x1; x < maxX; x++)
	{
		if(steep)
		{
			if(swx)
				q.push_back({x, y});
			else
			{
				map[x][y].seen = true;
				map[x][y].inView = true;
			}
			if( (map[x][y].tile == wall || map[x][y].tile == door) && !swx )
				return;
		}
		else
		{
			if(swx)
				q.push_back({y, x});
			else
			{
				map[y][x].seen = true;
				map[y][x].inView = true;
			}
			
			if( (map[y][x].tile == wall || map[y][x].tile == door) && !swx )
				return;
		}
		
		error -= dy;
		if(error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
	int qx, qy;
	while(!q.empty())
	{
		qy = q[q.size()-1].first;
		qx = q[q.size()-1].second;
		q.pop_back();
		map[qy][qx].seen = true;
		map[qy][qx].inView = true;
		if(map[qy][qx].tile == wall || map[qy][qx].tile == door)
			return;
	}
}

void ray(int x1, int y1, int x2, int y2)
{
	if(x1 == x2)
		yray(x1, y1, y2);
	else if(y1 == y2)
		xray(x1, x2, y2);
	else
		dray(x1, y1, x2, y2);
}

void entities()
{
	switch(map[rogue.y][rogue.x].tile)
	{
		case gold:
			rogue.gold++;
			map[rogue.y][rogue.x].tile = path;
			break;
		case key:
			rogue.keys++;
			map[rogue.y][rogue.x].tile = path;
			break;
		case door:
			if(!map[rogue.y][rogue.x].door_open && rogue.keys)
			{
				map[rogue.y][rogue.x].door_open = true;
				rogue.keys--;
			}
			break;
	}
}

void render_player()
{
	if(map[rogue.y][rogue.x].tile == edge)
		init_pair(player, COLOR_WHITE, COLOR_BLUE);
	else if(map[rogue.y][rogue.x].tile == door || map[rogue.y][rogue.x].tile == bridge)
		init_pair(player, COLOR_WHITE, COLOR_BROWN);
	else
		init_pair(player, COLOR_WHITE, COLOR_DARK_BLUE);
	attron(COLOR_PAIR(player));
	mvprintw(rogue.y, rogue.x + 25, "@");
}

void render()
{
	for(int i = 0; i < display.h; i++)
	{
		for(int j = 0; j < map_mx; j++)
		{
			map[i][j].inView = false;
		}
	}
	for(int i = 0; i < display.h; i++)
	{
		for(int j = 0; j < map_mx; j++)
		{
			if(!i || i == display.h - 1 || !j || j == map_mx - 1)
			{
				ray(rogue.x, rogue.y, j, i);
			}
		}
	}
	for(int i = 0; i < display.h; i++)
	{
		for(int j = 0; j < map_mx; j++)
		{
			if(!map[i][j].tile)
				continue;
			if(!map[i][j].seen)
				continue;
			if(map[i][j].inView)
				attron( COLOR_PAIR(map[i][j].tile) );
			else
				attron(COLOR_PAIR(map[i][j].tile + 20));
			switch(map[i][j].tile)
			{
				case door:
					mvprintw(i, j + 25, "%c", tileset[door][ map[i][j].door_open ]);
					break;
				default:
					mvprintw(i, j + 25, "%c", tileset[ map[i][j].tile ][rand() % tileset[ map[i][j].tile ].size() ]);
			}
			
		}
	}
	render_player();
}

void ui()
{
	attron(A_BOLD);
	attron(COLOR_PAIR(player));
	mvprintw(0, 0, "@");
	attron(COLOR_PAIR(text));
	mvprintw(0, 1, ": You, Level %d       ", rogue.level);
	attroff(A_BOLD);

	std::string sStats[3] = {"         Health         ", "        Nutrition       ", "       Experience       "};

	double hea = std::floor((double)rogue.health/rogue.maxhealth * 24);
	double nut = std::floor((double)rogue.nutr/rogue.maxnutr * 24);
	double ex = std::floor((double)rogue.exp/rogue.nlvl * 24);

	double dStats[3] = {hea, nut, ex};

	for(int bar = 0; bar < 3; bar++)
	{
		for(unsigned int i = 0; i < 24; i++)
		{
			if(i <= dStats[bar]) 
				attron( COLOR_PAIR(ui1 + (bar % 2) * 3) );
			else 
				attron( COLOR_PAIR(bui1 + (bar % 2) * 3) );
			mvprintw(bar + 1, i, "%c", sStats[bar][i]);
			if(bar == 2)
			{
				attron(A_BOLD);
				if(ex > 0) 
					attron( COLOR_PAIR(ui1) );
				mvprintw(bar + 1, 0, "%d", rogue.level);
				if(ex < 21)
					attron( COLOR_PAIR(bui1) );
				mvprintw(bar + 1, 24 - (rogue.level > 10) - 1, "%d", rogue.level + 1);
				attroff(A_BOLD);
			}
		}
	}

	attron(COLOR_PAIR(uitext));

	std::string s = "Str: " + std::to_string(rogue.strength) + " Armor: " + std::to_string(rogue.armor);
	mvprintw(4, (22 - s.length() ) / 2, "%s",  s.c_str());

	s = "Gold: " + std::to_string(rogue.gold) + " Keys: " + std::to_string(rogue.keys);
	mvprintw(5, (22 - s.length() ) / 2, "%s",  s.c_str());
	
	attron(A_BOLD);
	attron(COLOR_PAIR(text));
	s = "-- Depth: " + std::to_string(rogue.depth) + " --";
	mvprintw(display.h - 1, (22 - s.length() ) / 2, "%s", s.c_str());
	attroff(A_BOLD);
}

void lighting_test()
	{
		rogue.y = 20;
		rogue.x = 40;
		
		rect(10, 20, 23, 46, path, wall);
		rect(13, 30, 3, 6, zero, wall);
		rect(13, 50, 3, 6, zero, wall);
		rect(20, 30, 3, 6, zero, wall);
		rect(20, 50, 3, 6, zero, wall);
		rect(27, 30, 3, 6, zero, wall);
		rect(27, 50, 3, 6, zero, wall);
		return;
	}

int main()
{	
	if(!initscr()) 
	{
		printf("Error initializing screen.\n");
		exit(1);
	}
	if(!has_colors()) 
	{
		printf("This terminal does not support colours.\n");
		exit(1);
	}
	if(!can_change_color())
	{
		printf("Your terminal needs to support 256 colors\n");
		exit(1);
	}
	srand(time(NULL));
	getmaxyx(stdscr, display.h, display.w);
	map_mx = display.w - 25;
	keypad(stdscr, true);
	graphics_init();

	//test_map();
	lighting_test();
	//Game loop
	printw("Press any key to start");
	while(getMovement())
	{
		entities();
		render();
		ui();
	}

	endwin();
	return 0;
}
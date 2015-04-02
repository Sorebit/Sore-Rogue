#include "Graphics.h"
#include <ncurses.h>
#include <cstdlib>
#include <string>
#include <cmath>
#include <vector>

void wininit()
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
		printf("Your terminal needs to support 256 colors.\n");
		exit(1);
	}
}

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
	init_pair(stairs, COLOR_GOLD, COLOR_DARK_BLUE);

	init_pair(opath, COLOR_OOV_GREY, COLOR_OOV_BLUE);
	init_pair(ograss, COLOR_OOV_LIGHT_GREEN, COLOR_OOV_BLUE);
	init_pair(opit, COLOR_OOV_DARK_GREY, COLOR_BLACK);
	init_pair(oedge, COLOR_OOV_GREY, COLOR_DARK_BLUE);
	init_pair(obridge, COLOR_OOV_LIGHT_BROWN, COLOR_OOV_BROWN);
	init_pair(owall, COLOR_BLACK, COLOR_OOV_GREY);
	init_pair(ocoast, COLOR_GREY, COLOR_OOV_LIGHT_WATER);
	init_pair(odoor, COLOR_OOV_LIGHT_BROWN, COLOR_OOV_BROWN);
	init_pair(okey, COLOR_OOV_GOLD, COLOR_OOV_BLUE);
	init_pair(ostairs, COLOR_OOV_GOLD, COLOR_OOV_BLUE);

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

void show_equipment(Character & rogue)
{
	// NOTE: Item name can't be longer than 20 chars
	// An item class?
	attron(A_BOLD);
	attron(COLOR_PAIR(text));
	mvprintw(6, 0, "        Equipment       ");
	attroff(A_BOLD);

	attron(COLOR_PAIR(uitext));
	std::string s = "Silver: " + std::to_string(rogue.silver);
	mvprintw(7, 2 + (22 - s.length() ) / 2, "%s",  s.c_str());
	mvprintw(7, 0, "-");
	mvprintw(7, 23, "-");

	mvprintw(8, 0, "-      Canned meat     -");
	mvprintw(9, 0, "-       Rusty key      -");

}

void ui(Character rogue)
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
	mvprintw(4, 2 + (22 - s.length() ) / 2, "%s",  s.c_str());
	
	show_equipment(rogue);

	attron(A_BOLD);
	attron(COLOR_PAIR(text));
	s = "-- Depth: " + std::to_string(rogue.depth) + " --";
	mvprintw(maxy - 1, (22 - s.length() ) / 2, "%s", s.c_str());
	attroff(A_BOLD);

}

void ray(Tile map[][300], float x1, float y1, float x2, float y2)
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

	const int _maxX = (int)x2;

	for(int x = (int)x1; x < _maxX; x++)
	{
		if(steep)
		{
			if(swx)
				q.push_back({x, y});
			else
			{
				map[x][y].seen = true;
				map[x][y].inView = true;
				if(map[x][y].tile == wall || map[x][y].tile == door)
					return;
			}
		}
		else
		{
			if(swx)
				q.push_back({y, x});
			else
			{
				map[y][x].seen = true;
				map[y][x].inView = true;
				if(map[y][x].tile == wall || map[y][x].tile == door)
					return;
			}
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

void render_player(Tile map[][300], Character rogue)
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

void render(Tile map[][300], Character rogue)
{
	for(int i = 0; i < maxy; i++)
	{
		for(int j = 0; j < maxx; j++)
		{
			map[i][j].inView = false;
		}
	}
	for(int i = 0; i <= maxy; i++)
	{
		for(int j = 0; j <= maxx; j++)
		{
			if(!i || i == maxy || !j || j == maxx)
			{
				ray(map, rogue.x, rogue.y, j, i);
			}
		}
	}
	for(int i = 0; i < maxy; i++)
	{
		for(int j = 0; j < maxx; j++)
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
	render_player(map, rogue);
}
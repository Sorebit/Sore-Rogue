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
		endwin();
		exit(1);
	}
	if(!has_colors()) 
	{
		printf("This terminal does not support colours.\n");
		endwin();
		exit(1);
	}
	if(!can_change_color())
	{
		printf("Your terminal needs to support 256 colors.\n");
		endwin();
		exit(1);
	}
}

void graphics_init()
{
	start_color();

	// Make black be black
	init_color(0, 0, 0, 0);

	// Custom color definitons
	init_color(COLOR_BLUE, 45, 60, 350);
	init_color(COLOR_DARK_BLUE, 0, 0, 80);
	init_color(COLOR_GOLD, 1000, 900, 300);
	init_color(COLOR_LIGHT_GREEN, 615, 1000, 490);
	init_color(COLOR_BROWN, 322, 78, 78);
	init_color(COLOR_LIGHT_BROWN, 784, 560, 374);
	init_color(COLOR_WATER_BACK, 180, 243, 1000);
	init_color(COLOR_WATER_FORE, 54, 86, 1000);
	init_color(COLOR_GREY, 700, 700, 700);
	init_color(COLOR_COAST, 639, 639, 1000);

	init_color(COLOR_OOV_GREY, 300, 300, 370);
	init_color(COLOR_OOV_DARK_GREY, 150, 150, 220);
	init_color(COLOR_OOV_BLUE, 0, 0, 30);
	init_color(COLOR_OOV_WATER_FORE, 0, 0, 800);
	init_color(COLOR_OOV_WATER_BACK, 0, 43, 800);
	init_color(COLOR_OOV_LIGHT_GREEN, 215, 400, 90);
	init_color(COLOR_OOV_GOLD, 400, 400, 0);
	init_color(COLOR_OOV_BROWN, 70, 20, 20);
	init_color(COLOR_OOV_LIGHT_BROWN, 284, 150, 150);
	init_color(COLOR_OOV_COAST, 389, 389, 750);

	init_color(COLOR_UI1, 141, 188, 710);
	init_color(COLOR_BUI1, 0, 0, 250);
	init_color(COLOR_UI2, 41, 88, 610);
	init_color(COLOR_BUI2, 0, 0, 150);
	init_color(COLOR_UI3, 450, 0, 0);
	init_color(COLOR_BUI3, 300, 0, 0);

	// Custom color pairs

	// Path & Walls
	init_pair(path, COLOR_GREY, COLOR_DARK_BLUE);
	init_pair(grass, COLOR_LIGHT_GREEN, COLOR_DARK_BLUE);
	init_pair(pit, COLOR_OOV_GREY, COLOR_BLACK);
	init_pair(edge, COLOR_WHITE, COLOR_BLUE);
	init_pair(bridge, COLOR_LIGHT_BROWN, COLOR_BROWN);
	init_pair(wall, COLOR_BLACK, COLOR_WHITE);
	init_pair(coast, COLOR_WHITE, COLOR_COAST);
	init_pair(door, COLOR_LIGHT_BROWN, COLOR_BROWN);
	init_pair(key, COLOR_GOLD, COLOR_DARK_BLUE);
	init_pair(stairs, COLOR_GOLD, COLOR_DARK_BLUE);

	init_pair(opath, COLOR_OOV_GREY, COLOR_OOV_BLUE);
	init_pair(ograss, COLOR_OOV_LIGHT_GREEN, COLOR_OOV_BLUE);
	init_pair(opit, COLOR_OOV_DARK_GREY, COLOR_BLACK);
	init_pair(oedge, COLOR_OOV_GREY, COLOR_DARK_BLUE);
	init_pair(obridge, COLOR_OOV_LIGHT_BROWN, COLOR_OOV_BROWN);
	init_pair(owall, COLOR_BLACK, COLOR_OOV_GREY);
	init_pair(ocoast, COLOR_GREY, COLOR_OOV_COAST);
	init_pair(odoor, COLOR_OOV_LIGHT_BROWN, COLOR_OOV_BROWN);
	init_pair(okey, COLOR_OOV_GOLD, COLOR_OOV_BLUE);
	init_pair(ostairs, COLOR_OOV_GOLD, COLOR_OOV_BLUE);

	// Living creatures
	init_pair(player, COLOR_WHITE, COLOR_DARK_BLUE);
	init_pair(mob, COLOR_GOLD, COLOR_DARK_BLUE);
	init_pair(omob, COLOR_OOV_GOLD, COLOR_OOV_BLUE);

	// Fluids
	init_pair(water, COLOR_WATER_FORE, COLOR_WATER_BACK);
	init_pair(owater, COLOR_OOV_WATER_FORE, COLOR_OOV_WATER_BACK);

	// Other
	init_pair(gold, COLOR_GOLD, COLOR_DARK_BLUE);
	init_pair(ogold, COLOR_OOV_GOLD, COLOR_OOV_BLUE);

	init_pair(text, COLOR_WHITE, COLOR_BLACK);
	init_pair(ui1, COLOR_GREY, COLOR_UI1);
	init_pair(bui1, COLOR_GREY, COLOR_BUI1);
	init_pair(ui2, COLOR_GREY, COLOR_UI2);
	init_pair(bui2, COLOR_GREY, COLOR_BUI2);
	init_pair(ui3, COLOR_GREY, COLOR_UI3);
	init_pair(bui3, COLOR_GREY, COLOR_BUI3);
	init_pair(uitext, COLOR_GREY, COLOR_BLACK);

	// Cursor and invisible input
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

void show_mobs_nearby(Character rogue, std::vector <Mob> mob_list)
{
	int offset = 0;
	for(unsigned int i = 0; i < mob_list.size(); i++)
	{
		if(!mob_list[i].seesPlayer(rogue))
			continue;

		attron(COLOR_PAIR(mob));
		mvprintw(11 + offset, 0, "%c", mob_list[i].getTile());
		attron(COLOR_PAIR(text));
		mvprintw(11 + offset, 1, ": %s", mob_list[i].getName().c_str());

		int health = mob_list[i].getHealth().first;
		int maxhealth = mob_list[i].getHealth().second;
		std::string healStr = "         Health         ";
		double percent = std::floor((double)health/maxhealth * 24);

		for(unsigned int i = 0; i < 24; i++)
		{
			attron( COLOR_PAIR(ui3 + (i > percent) ) );
			mvprintw(12 + offset, i, "%c", healStr[i]);
		}
		
		offset += 3;
	}
}

void ui(Character rogue, std::vector <Mob> mob_list)
{
	// Clear the bar first
	attron(COLOR_PAIR(uitext));
	for(int y = 0; y < maxy; y++)
	{
		for(int x = 0; x < 25; x++)
		{
			mvprintw(y, x, " ");
		}
	}

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
	mvprintw(4, 1 + (22 - s.length() ) / 2, "%s",  s.c_str());
	
	show_equipment(rogue);

	show_mobs_nearby(rogue, mob_list);

	attron(A_BOLD);
	attron(COLOR_PAIR(text));
	s = "-- Depth: " + std::to_string(rogue.depth) + " --";
	mvprintw(maxy - 1, (22 - s.length() ) / 2, "%s", s.c_str());
	attroff(A_BOLD);

}

void ray(float x1, float y1, float x2, float y2)
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

	const int max_x = (int)x2;

	const int max_length = 16; // testing 
	int length = 0; // testing

	for(int x = (int)x1; x < max_x; x++)
	{
		if(length > max_length && !swx) // testing
			return; // testing
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
		length++; // testing
	}

	int qx, qy;
	length = 0;
	while(!q.empty())
	{
		if(length++ > max_length) // testing
			return; // testing
		qy = q[q.size()-1].first;
		qx = q[q.size()-1].second;
		q.pop_back();
		map[qy][qx].seen = true;
		map[qy][qx].inView = true;
		if(map[qy][qx].tile == wall || map[qy][qx].tile == door)
			return;
	}
	
	if(swx)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	if(!steep)
	{
		map[(int)y1][(int)x1].seen = true;
		map[(int)y1][(int)x1].inView = true;
	}
	else
	{
		map[(int)x1][(int)y1].seen = true;
		map[(int)x1][(int)y1].inView = true;
	}
}

void render_player(Character rogue)
{
	if(map[rogue.y][rogue.x].tile == edge)
		init_pair(player, COLOR_WHITE, COLOR_BLUE);
	else if(map[rogue.y][rogue.x].tile == door || map[rogue.y][rogue.x].tile == bridge)
		init_pair(player, COLOR_WHITE, COLOR_BROWN);
	else if(map[rogue.y][rogue.x].tile == coast)
		init_pair(player, COLOR_WHITE, COLOR_COAST);
	else
		init_pair(player, COLOR_WHITE, COLOR_DARK_BLUE);
	attron(COLOR_PAIR(player));
	mvprintw(rogue.y, rogue.x + 25, "@");
}

void render_mobs(Character rogue, std::vector <Mob> mob_list)
{
	for(unsigned int i = 0; i < mob_list.size(); i++)
	{
		std::pair <int, int> pos = mob_list[i].getPos();
		if(!map[pos.first][pos.second].seen)
				continue;
		if(mob_list[i].seesPlayer(rogue))
		{
			attron(COLOR_PAIR(mob));
			mvprintw(pos.first, pos.second + 25, "%c", mob_list[i].getTile());
		}
	}
}

void render(Character rogue, std::vector <Mob> mob_list)
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
				ray(rogue.x, rogue.y, j, i);
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
	render_mobs(rogue, mob_list);
	render_player(rogue);
}
// Sorbet - 2015
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
		printf("Your terminal does not support colours.\n");
		endwin();
		exit(1);
	}
	if(!can_change_color())
	{
		printf("Your terminal needs to support 256 colors.\n");
		endwin();
		exit(1);
	}

	keypad(stdscr, true);
	getmaxyx(stdscr, maxy, maxx);
	if(maxy > 300 || maxx > 300)
	{
		printw("Your terminal screen is bigger than 300x300\nPlease reduce its size\n");
		getch();
		endwin();
		exit(1);
	}
	if(maxy < 24 || maxx < 80)
	{
		printw("Your terminal screen is %dx%d\n", maxx, maxy);
		printw("It should be at least 80x24\n");
		printw("Please increse its size\n");
		getch();
		endwin();
		exit(1);
	}
	maxx -= 25;
	maxy -= 3;
}

void graphics_init()
{
	start_color();

	// Make black black
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
	init_color(COLOR_UI3, 500, 150, 100);
	init_color(COLOR_BUI3, 300, 0, 0);
	init_color(COLOR_EQ, 20, 20, 20);

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

	// Status bar colors
	init_pair(text, COLOR_WHITE, COLOR_BLACK);
	init_pair(ui1, COLOR_GREY, COLOR_UI1);
	init_pair(dark_ui1, COLOR_GREY, COLOR_BUI1);
	init_pair(ui2, COLOR_GREY, COLOR_UI2);
	init_pair(dark_ui2, COLOR_GREY, COLOR_BUI2);

	// Mob health bar colors
	init_pair(ui3, COLOR_GREY, COLOR_UI3);
	init_pair(dark_ui3, COLOR_GREY, COLOR_BUI3);
	init_pair(uitext, COLOR_GREY, COLOR_BLACK);

	init_pair(eq1, COLOR_WHITE, COLOR_EQ);

	// Cursor and invisible input
	curs_set(0);
	noecho();
}

void quick_use()
{
	attron(A_BOLD);
	attron(COLOR_PAIR(text));
	mvprintw(6, 0, "        Quick use       ");
	attroff(A_BOLD);

	attron(COLOR_PAIR(uitext));
	mvprintw(7, 0, "- a  Lesser Potion     -");	
	mvprintw(8, 0, "- s  Greater Potion    -");
	mvprintw(9, 0, "- d  Weakness Potion   -");

}

void show_mobs_nearby(int _off)
{
	int offset = _off;
	for(unsigned i = 0; i < mob_list.size() && (5 + offset) < maxy - 1; i++)
	{
		if(!mob_list[i].seesPlayer(rogue))
			continue;

		attron(COLOR_PAIR(mob));
		mvprintw(3 + offset, 0, "%c", mob_list[i].getTile());
		attron(COLOR_PAIR(text));
		mvprintw(3 + offset, 1, ": %s", mob_list[i].getName().c_str());

		int health = mob_list[i].getHealth().first;
		int maxhealth = mob_list[i].getHealth().second;
		std::string healStr = "         Health         ";
		double percent = std::floor((double)health/maxhealth * 24);

		for(unsigned i = 0; i < 24; i++)
		{
			attron( COLOR_PAIR(ui3 + (i > percent) ) );
			mvprintw(4 + offset, i, "%c", healStr[i]);
		}
		
		offset += 3;
	}
}

void ui()
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

	// Show basic info: color, level
	attron(A_BOLD);
	attron(COLOR_PAIR(player));
	mvprintw(0, 0, "@");
	attron(COLOR_PAIR(text));
	mvprintw(0, 1, ": You, Level %d       ", rogue.level);
	attroff(A_BOLD);

	std::string sStats[] = {"         Health         ", "       Experience       ", 
							"        Poisoned        ", "          Weak          ", 
							"        Confused        ", "        Invisible       "};

	// Calculate the percentage of bar used by a stat
	double hea = std::floor((double)rogue.health/rogue.maxhealth * 24);
	double ex = std::floor((double)rogue.exp/rogue.nlvl * 24);
	double poi = std::floor((double)rogue.status[poisoned]/50 * 24);
	double wea = std::floor((double)rogue.status[weak]/35 * 24);
	double con = std::floor((double)rogue.status[confused]/25 * 24);
	double inv = std::floor((double)rogue.status[invisible]/25 * 24);

	double dStats[] = {hea, ex, poi, wea, con, inv};

	int bar = 0;

	for(int st = 0; st < 6; ++st)
	{
		if(st >= 2 && dStats[st] <= 0)
			continue;
		
		int col = (bar % 2) ? ui2 : ui1;
		bool changed = false;

		for(unsigned i = 0; i < 24; i++)
		{
			if(i > (unsigned)dStats[st] && !changed)
			{
				changed = true;
				++col;
			}

			attron( COLOR_PAIR(col) );
				
			mvprintw(bar + 1, i, "%c", sStats[st][i]);

		}

		// 0 health display fix
		if(!bar && hea == 0)
		{
			attron(COLOR_PAIR(dark_ui1));
			mvprintw(1, 0, " ");
		}

		// Add levels to experience bar
		if(bar == 1)
		{
			attron(A_BOLD);
			if(ex > 0) 
				attron( COLOR_PAIR(ui2) );
			mvprintw(bar + 1, 0, "%d", rogue.level);
			if(ex < 21)
				attron( COLOR_PAIR(dark_ui2) );
			mvprintw(bar + 1, 24 - (rogue.level > 10) - 1, "%d", rogue.level + 1);
			attroff(A_BOLD);
		}
		bar++;
	}

	attron(COLOR_PAIR(uitext));

	std::string s = "Str: " + std::to_string(rogue.strength) + " Def: " + std::to_string(rogue.defense);
	mvprintw(bar + 1, 1 + (22 - s.length() ) / 2, "%s",  s.c_str());
	
	//quick_use();

	show_mobs_nearby(bar);

	attron(A_BOLD);
	attron(COLOR_PAIR(text));
	s = "-- Depth: " + std::to_string(rogue.depth) + " --";
	mvprintw(maxy + 2, (22 - s.length() ) / 2, "%s", s.c_str());
	attroff(A_BOLD);


	// __DEBUG__
	mvprintw(1, 0, "%d/%d", rogue.health, rogue.maxhealth);
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

	const int max_length = 16; 
	int length = 0;

	for(int x = (int)x1; x < max_x; x++)
	{
		if(length > max_length && !swx)
			return;
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
		length++;
	}

	int qx, qy;
	length = 0;
	while(!q.empty())
	{
		if(length++ > max_length)
			return;
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

void render_player()
{
	int fg, bg;

	if(rogue.status[invisible] > 0)
		fg = COLOR_OOV_GREY;
	else
		fg = COLOR_WHITE;

	if(map[rogue.y][rogue.x].tile == edge)
		bg = COLOR_BLUE;
	else if(map[rogue.y][rogue.x].tile == door || map[rogue.y][rogue.x].tile == bridge)
		bg = COLOR_BROWN;
	else if(map[rogue.y][rogue.x].tile == coast)
		bg = COLOR_COAST;
	else
		bg = COLOR_DARK_BLUE;

	init_pair(player, fg, bg);
	attron(COLOR_PAIR(player));
	mvprintw(rogue.y, rogue.x + 25, "@");
}

void render_mobs()
{
	for(unsigned i = 0; i < mob_list.size(); i++)
	{
		std::pair <int, int> pos = mob_list[i].getPos();
		if(!map[pos.first][pos.second].seen)
				continue;
		//if(mob_list[i].seesPlayer(rogue))
		{
			attron(COLOR_PAIR(mob));
			mvprintw(pos.first, pos.second + 25, "%c", mob_list[i].getTile());
		}
	}
}

void render()
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
	render_mobs();
	render_player();
}

void message(std::string mes)
{
	// Do not display empty messages
	if(mes == "")
		return;

	// Clear the padding first
	attron(COLOR_PAIR(uitext));
	for(int y = maxy; y < maxy + 3; y++)
		for(int x = 25; x < maxx + 25; x++)
			mvprintw(y, x, " ");

	mvprintw(maxy, 25, messages_old[0].c_str());
	mvprintw(maxy + 1, 25, messages_old[1].c_str());
	
	// Most recent is the brightest
	attron(COLOR_PAIR(text));
	mvprintw(maxy + 2, 25, mes.c_str());

	// Update buffer
	messages_old[0] = messages_old[1];
	messages_old[1] = mes;
}

void erase_current_item()
{
	//If item has more that one copy, simply decrease its quantity
	if(items[sel].getQuantity() > 0)
	{
		items[sel].setQuantity(items[sel].getQuantity() - 1);
		if(items[sel].getQuantity() > 0)
			return;
	}
	// Don't erase an inexistant item
	if(items.empty())
		return;

	// Erase an item from the list 
	if( sel == items.size() - 1 && !items.empty()) 
	{
		items.pop_back();
		// Move cursor to the item above, if exists
		if(!items.empty())
			--sel; 
	}
	else
	{
		items.erase( items.begin() + sel );
	}

	// Scrolling up when erasing
	if(offset > 0 && sel - offset == 0)
	{
		--sel;
		--offset;
	}
}

void equipment()
{
	int _up =  maxy/2 - 10;
	int _left = (maxx + 24)/2 - 26;
	unsigned opt = 0;
	unsigned opts[] = { 0, 7, 15};
	bool esc = false;

	while(!esc)
	{
		// Draw the window box
		attron(COLOR_PAIR(eq1));
		for(int y = _up; y < _up + 21; y++)
		{
			for(int x = _left; x < _left + 52; x++)
			{
				mvprintw(y, x, " ");
			}
		}
		// Show window name and equipped items
		mvprintw(_up + 1, _left + 22, "Equipment");

		mvprintw(_up + 3, _left + 2, "Wep: %s", rogue.wep_eq.getName().c_str());
		mvprintw(_up + 4, _left + 2, "Arm: %s", rogue.arm_eq.getName().c_str());
		mvprintw(_up + 5, _left + 2, "Spc: %s", rogue.spc_eq.getName().c_str());

		mvprintw(_up + 3, _left + 37, "Attack:   %s", std::to_string(rogue.attack).c_str());
		mvprintw(_up + 4, _left + 37, "Defense:  %s", std::to_string(rogue.defense).c_str());
		mvprintw(_up + 5, _left + 37, "Strength: %s", std::to_string(rogue.strength).c_str());


		// Draw the item list depending on cursor
		if(!items.empty())
		{
			for(unsigned it = 0 + offset; it < items.size(); ++it)
			{
				if(it - offset > 8)
					break;
				std::string str = items[it].getName();
				if(items[it].getQuantity() > 1)
					str += " (" + std::to_string(items[it].getQuantity()) + ")";
				mvprintw(_up + 7 + it - offset, _left + 4, str.c_str());
			}

			// Display basic info about selected item
			mvprintw(_up + 17, _left + 2, "                                             ");
			mvprintw(_up + 17, _left + 2, items[sel].getSummary().c_str());
		}
		else
		{
			mvprintw(_up + 7, _left + 4, "Your backpack is empty.");
		}

		// Drawn item cursor
		if(sel - offset <= 8)
			mvprintw(_up + 7 + sel - offset, _left + 2, ">");

		// Draw actions and action cursor
		mvprintw(_up + 19, _left + 2, "  Use    Toss    Sort");
		mvprintw(_up + 19, _left + 2 + opts[opt], ">");

		int key = getch();
		switch(key)
		{
		case up:
			// Move item cursor up
			if(items.empty())
				break;
			if(sel)
				--sel;
			if((int)(sel - offset) < 0)
				--offset;
			break;
		case down:
			// Move item cursor down
			if(items.empty())
				break;
			if(sel < items.size() - 1)
				++sel;
			if(sel - offset > 8)
				++offset;
			break;
		case left:
			// Move action cursor left
			if(opt)
				--opt;
			break;
		case right:
			// Move action cursor left
			if(opt < 2)
				++opt;
			break;
		case enter: case 'w':
			// Execute selected action
			switch(opt)
			{
			case 0:
				// Use item, I think I check too often if items exist
				if(items.empty())
					break;
				items[sel].use();
				if(!items.empty() && items[sel].getName() == "Not equipped")
					erase_current_item();
				break;

			case 1:
				// Toss item
				if(items.empty())
					break;
				items[sel].toss();
				erase_current_item();
				break;

			case 2:
				// Sort backpack
				if(items.empty())
					break;
				std::sort(items.begin(), items.end(), compare_items);
				message("Items sorted. Not really");
				break;
			}
			break;
		case 'e':
			// Close equipment window 
			esc  = true;
		}
	}

	// Clear the screen up
	attron(COLOR_PAIR(text));
	for(int y = _up; y < _up + 21; y++)
	{
		for(int x = _left; x < _left + 52; x++)
		{
			mvprintw(y, x, " ");
		}
	}
}
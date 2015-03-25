#include "Graphics.h"
#include <ncurses.h>
#include <cstdlib>

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
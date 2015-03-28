#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Globals.h"

int wininit();
void graphics_init();
void show_equipment(Character & rogue);
void ui(Character rogue);
void ray(Tile map[][200], float x1, float y1, float x2, float y2);
void render_player(Tile map[][200], Character rogue);
void render(Tile map[][200], Character rogue);

#endif
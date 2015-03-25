#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Globals.h"

void wininit();
void graphics_init();
void ui(Disp display, Character rogue);

void ray(Tile map[][200], float x1, float y1, float x2, float y2);

#endif
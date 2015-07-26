// Sorbet - 2015
#pragma once

#include "Globals.h"
#include "Mob.h"

void wininit();
void graphics_init();
void show_equipment();
void show_mobs_nearby();
void ui();
void ray(float x1, float y1, float x2, float y2);
void render_mobs();
void render_player();
void render();
void message(std::string text);

extern std::string messages_old[2];
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Globals.h"
#include "Mob.h"

void wininit();
void graphics_init();
void show_equipment(Character & rogue);
void show_mobs_nearby(Character rogue, std::vector <Mob> mob_list);
void ui(Character rogue, std::vector <Mob> mob_list);
void ray(float x1, float y1, float x2, float y2);
void render_mobs(Character rogue, std::vector <Mob> mob_list);
void render_player(Character rogue);
void render(Character rogue, std::vector <Mob> mob_list);

#endif
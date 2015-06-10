#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Globals.h"
#include "Mob.h"

void wininit();
void graphics_init();
void show_equipment(Character & rogue);
void show_mobs_nearby(Tile map[][300], Character rogue, std::vector <Mob> mob_list);
void ui(Tile map[][300], Character rogue, std::vector <Mob> mob_list);
void ray(Tile map[][300], float x1, float y1, float x2, float y2);
void render_mobs(Tile map[][300], Character rogue, std::vector <Mob> mob_list);
void render_player(Tile map[][300], Character rogue);
void render(Tile map[][300], Character rogue, std::vector <Mob> mob_list);

#endif
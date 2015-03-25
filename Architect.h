#ifndef ARCHITECT_H
#define ARCHITECT_H

#include "Globals.h"

void rect(Tile map[][200], int y, int x, int h, int w, int t1, int t2, bool corners = true, int t3 = 0);

void lighting_test(Tile tmap[][200], Character & crogue);

void test_map(Tile tmap[][200], Character & crogue);

#endif
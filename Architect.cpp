#include "Architect.h"

#include <ncurses.h>
#include <cstdlib>
#include <ctime>

void init_map(Tile map[][200])
{
	max_lake = max_lake_size = comp_count = 0;
	for(int i = 0; i < 200; i++)
		count[i] = 0;
	for(int y = 0; y < maxy; y++)
	{
		for(int x = 0; x < maxx; x++)
		{
			comp[y][x] = 0;
			map[y][x].seen = 0;
			map[y][x].inView = 0;
			map[y][x].door_open = 0;

			map[y][x].tile = 0;
			if(rand() % 100 < 55)
				map[y][x].tile = 1;
		}
	}
}

int count_neighbours(int y, int x, Tile map[][200])
{
	int n = 0;
	for(int i = -1; i < 2; i++)
	{
		for(int j = -1; j < 2; j++)
		{
			if(i == j && i == 0)
				continue;
			if( (y + i) < 0 || (y + i) > maxy - 1)
				continue;
			if( (x + j) < 0 || (x + j) > maxx - 1)
				continue;
			if(map[y + i][x + j].tile)
				n++; 
		}
	}
	return n;
}

void step(Tile map[][200])
{
	int temp[200][200];
	for(int y = 0; y < maxy; y++)
	{
		for(int x = 0; x < maxx; x++)
		{
			if(!map[y][x].tile)
				temp[y][x] = (count_neighbours(y, x, map) > 5);
			else
				temp[y][x] = !(count_neighbours(y, x, map) < 4);
		}
	}

	for(int y = 0; y < maxy; y++)
	{
		for(int x = 0; x < maxx; x++)
		{
			map[y][x].tile = temp[y][x];
		}
	}
}

void dfs(int y, int x, Tile map[][200])
{
	int nei[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
	comp[y][x] = comp_count;
	count[comp_count]++;
	for(int i = 0; i < 4; i++)
	{
		int ny = y + nei[i][0];
		int nx = x + nei[i][1];
		if(ny < 0 || ny > maxy - 1)
			continue;
		if(nx < 0 || nx > maxx - 1)
			continue;
		if(!map[ny][nx].tile)
			continue;
		if(!comp[ny][nx] && map[ny][nx].tile)
			dfs(ny, nx, map);
	}
}

void fill(Tile map[][200])
{
	for(int y = 0; y < maxy; y++)
	{
		for(int x = 0; x < maxx; x++)
		{	
			if(!comp[y][x] && map[y][x].tile)
			{
				comp_count++;
				dfs(y, x, map);
			}
		}
	}
}


void borders(Tile map[][200])
{
	bool fout = false;
	for(int y = 0; y < maxy; y++)
	{
		for(int x = 0; x < maxx; x++)
		{
			if(map[y][x].tile != 1)
				continue;
			for(int i = -1; i < 2; i++)
			{
				for(int j = -1; j < 2; j++)
				{
					int ny = y + i;
					int nx = x + j;
					if(ny < 0)
					{
						fout = true;
						ny++;
					}
					if(nx < 0)
					{
						fout = true;
						nx++;
					}
					if(ny > maxy - 1)
					{
						fout = true;
						ny--;
					}
					if(nx > maxx - 1)
					{
						fout = true;
						nx--;
					}
					if(!map[ny][nx].tile || fout)
						map[ny][nx].tile = wall;
					fout = false;
				}	
			}
		}
	}
}

void generate_lake(Tile map[][200])
{	
	init_map(map);
	for(int steps = 0; steps < 5; steps++)
		step(map);
	fill(map);

	for(int i = 1; i <= comp_count; i++)
	{
		if(count[i] >= max_lake_size)
		{
			max_lake_size = count[i];
			max_lake = i;
		}
	}
	for(int y = 0; y < maxy; y++)
	{
		for(int x = 0; x < maxx; x++)
		{
			if(comp[y][x] != max_lake)
				map[y][x].tile = 0;
		}
	}
	borders(map);
}

void put_stairs(Tile map[][200], Character & rogue)
{
	int ex, ey, walls = 0;
	while(true)
	{
		ex = rand() % maxx;
		ey = rand() % maxy;
		if(map[ey][ex].tile != path)
			continue;
		for(int i = -1; i < 2; i++)
		{
			for(int j = -1; j < 2; j++)
			{
				if(map[ey + i][ex + j].tile == wall)
					walls++;
			}
		}
		if(walls >= 5)
			break;
		else
			walls = 0;
	}
	rogue.y = ey;
	rogue.x = ex;
	
	int qx, qy, mqy = 0, mqx = 0, max_dist = 0;
	walls = 0;

	for(qy = 0; qy < maxy; qy++)
	{
		for(qx = 0; qx < maxx; qx++)
		{
			if(map[qy][qx].tile != path)
				continue;
			for(int i = -1; i < 2; i++)
			{
				for(int j = -1; j < 2; j++)
				{
					if(map[qy + i][qx + j].tile == wall)
						walls++;
				}
			}
			if(walls < 5)
				continue;
			if(max_dist < ( (qx-ex)*(qx-ex) + (qy-ey)*(qy-ey) ) )
			{
				max_dist = (qx-ex)*(qx-ex) + (qy-ey)*(qy-ey);
				mqy = qy;
				mqx = qx;
			}
		}
	}

	map[mqy][mqx].tile = stairs;
	map[mqy][mqx].seen = true;
}

void generate_dungeon(Tile map[][200], Character & rogue)
{
	do 
	{ 
		generate_lake(map);
	} while(max_lake_size < 1500);
	put_stairs(map, rogue);
}



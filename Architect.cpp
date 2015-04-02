#include "Architect.h"

#include <ncurses.h>
#include <cstdlib>
#include <ctime>

void init_map(Tile map[][300])
{
	max_lake = max_lake_size = comp_count = 0;
	for(int i = 0; i < 300; i++)
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

int count_neighbours(int y, int x, Tile map[][300])
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

void step(Tile map[][300])
{
	int temp[300][300];
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

void dfs(int y, int x, Tile map[][300])
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

void fill(Tile map[][300])
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
}


void borders(Tile map[][300], int tile)
{
	bool fout = false;
	for(int y = 0; y < maxy; y++)
	{
		for(int x = 0; x < maxx; x++)
		{
			if(map[y][x].tile != 1 && map[y][x].tile != grass)
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
						map[ny][nx].tile = tile;
					fout = false;
				}	
			}
		}
	}
}

void generate_lake(Tile map[][300])
{	
	init_map(map);
	for(int steps = 0; steps < 5; steps++)
		step(map);
	fill(map);
}

void put_stairs(Tile map[][300], Character & rogue)
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
	cx = mqx;
	cy = mqy;
}

void generate_grass(Tile map[][300])
{
	Tile blob[300][300];
	
	do
	{
		init_map(blob);
		for(int steps = 0; steps < 5; steps++)
			step(blob);
		fill(blob);
	} while(max_lake_size > (int)(maxy * maxx * 0.07));

	for(int y = 0; y < maxy; y++)
	{
		for(int x = 0; x < maxx; x++)
		{
			if(blob[y][x].tile && map[y][x].tile)
			{
				map[y][x].tile = grass;
			}
		}
	}
}

void generate_dungeon(Tile map[][300], Character & rogue)
{
	do 
	{ 
		generate_lake(map);
	} while(max_lake_size < (int)(maxy * maxx * 0.3) );
	
	if(rogue.depth < 3)
		generate_grass(map);
	borders(map, wall);
	put_stairs(map, rogue);
}
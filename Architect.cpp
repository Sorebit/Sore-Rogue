#include "Architect.h"

#include <ncurses.h>
#include <cstdlib>
#include <ctime>

extern int flood_been[300][300];
int flood_been[300][300];
bool possible;

void init_blob(Tile map[][300], int size_y, int size_x)
{
	max_lake = max_lake_size = comp_count = 0;
	for(int i = 0; i < 300; i++)
		count[i] = 0;
	for(int y = 0; y < size_y; y++)
	{
		for(int x = 0; x < size_x; x++)
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

void step(Tile map[][300], int my, int mx)
{
	int temp[300][300];
	for(int y = 0; y < my; y++)
	{
		for(int x = 0; x < mx; x++)
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

void fill(Tile map[][300], int my, int mx)
{
	for(int y = 0; y < my; y++)
	{
		for(int x = 0; x < mx; x++)
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
	for(int y = 0; y < my; y++)
	{
		for(int x = 0; x < mx; x++)
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
			if(map[y][x].tile == tile)
				continue;
			if(map[y][x].tile != path && map[y][x].tile != grass && map[y][x].tile != water && map[y][x].tile != coast && map[y][x].tile != pit && map[y][x].tile != edge)
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

void put_blob(Tile map[][300], int my, int mx, Tile blob[][300], int blob_y, int blob_x, int pos_y, int pos_x, bool passEmpty)
{
	for(int y = 0; y < blob_y; y++)
	{
		for(int x = 0; x < blob_x; x++)
		{
			if(y + pos_y < 0 || y + pos_y >= my)
				continue;
			if(x + pos_x < 0 || x + pos_x >= mx)
				continue;
			if(passEmpty || blob[y][x].tile)
			{
				map[y + pos_y][x + pos_x].tile = blob[y][x].tile;
			}	
		}
	}
}

void convert_tile(Tile map[][300], int my, int mx, int from, int to)
{
	for(int y = 0; y < my; y++)
	{
		for(int x = 0; x < mx; x++)
		{	
			if(map[y][x].tile == from)
				map[y][x].tile = to;
		}
	}
}

void generate_blob(Tile map[][300], int my, int mx)
{	
	init_blob(map, my, mx);
	for(int steps = 0; steps < 5; steps++)
		step(map, my, mx);
	fill(map, my, mx);
}

void flood(int y, int x, Tile map[][300])
{
	int nei[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
	flood_been[y][x] = true;

	for(int i = 0; i < 4; i++)
	{
		int ny = y + nei[i][0];
		int nx = x + nei[i][1];
		if(ny < 0 || ny > maxy - 1)
			continue;
		if(nx < 0 || nx > maxx - 1)
			continue;

		if(map[ny][nx].tile == stairs)
			possible = true;

		if(!map[ny][nx].tile || map[ny][nx].tile == pit || map[ny][nx].tile == water)
			flood_been[ny][nx] = true;

		if(!flood_been[ny][nx])
			flood(ny, nx, map);
	}
}

void put_stairs(Tile map[][300], Character & rogue)
{
	int ex, ey, walls = 0, walkables = 0;
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
				if(map[ey + i][ex + j].tile == path || map[ey + i][ex + j].tile == grass || map[ey + i][ex + j].tile == edge || map[ey + i][ex + j].tile == coast)
					walkables++;
			}
		}
		if(walls >= 5 && walls + walkables >= 7)
			break;
		else
			walls = 0;
	}
	rogue.y = ey;
	rogue.x = ex;
	
	while(true)
	{
		int qx, qy, mqy = 0, mqx = 0, max_dist = 0;
		walls = 0;
		possible = false;

		for(int i = 0; i < 300; i++)
		{
			for(int j = 0; j < 300; j++)
			{
				flood_been[i][j] = false;
			}
		}

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
		flood(rogue.y, rogue.x, map);
		if(possible)
			return;
		map[mqy][mqx].tile = wall;
	}
}

void generate_grass_pit(Tile map[][300], Character & rogue)
{
	Tile blob[300][300];
	int blob_y = 12, blob_x = 24, max_blobs = 3, tile;
	
	if(rogue.depth < 3)
		tile = grass; 
	else
		tile = pit;

	for(int bnum = 0; bnum < max_blobs; bnum++)
	{
		do
		{
			generate_blob(blob, blob_y, blob_x);
		} while(max_lake_size > (int)(maxy * maxx * 0.07));

		convert_tile(blob, blob_y, blob_x, path, tile);
		if(tile == pit)
			borders(blob, edge);
		put_blob(map, maxy, maxx, blob, blob_y, blob_x, rand() % (maxy - blob_y), rand() % (maxx - blob_x));
	}
}

void generate_lake(Tile map[][300])
{
	Tile lake[300][300];
	int lake_y = 16, lake_x = 34;
	generate_blob(lake, lake_y, lake_x);
	convert_tile(lake, lake_y, lake_x, path, water);
	borders(lake, coast);
	put_blob(map, maxy, maxx, lake, lake_y, lake_x, rand() % (maxy - lake_y), rand() % (maxx - lake_x));
}

void generate_pit(Tile map[][300])
{
	Tile tpit[300][300];
	int pit_y = 10, pit_x = 20;
	generate_blob(tpit, pit_y, pit_x);
	convert_tile(tpit, pit_y, pit_x, path, pit);
	borders(tpit, edge);
	put_blob(map, maxy, maxx, tpit, pit_y, pit_x, rand() % (maxy - pit_y), rand() % (maxx - pit_x));
}

void generate_dungeon(Tile map[][300], Character & rogue)
{
	do 
	{ 
		generate_blob(map, maxy, maxx);
	} while(max_lake_size < (int)(maxy * maxx * 0.25) );
	
	generate_grass_pit(map, rogue);
	if(rand() % 100 < 30)
	{
		generate_lake(map);
	}
	borders(map, wall);
	put_stairs(map, rogue);
}
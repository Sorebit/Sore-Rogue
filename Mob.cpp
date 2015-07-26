// Sorbet - 2015
#include "Mob.h"

Mob::Mob(std::string species)
{
	// TODO:
	// more species
	// more attributes (experience that they drop)
	// special skills ?
	// 

	std::transform(species.begin(), species.end(), species.begin(), ::tolower);
	
	if(species == "rat")
	{
		name = "Rat";
		tile = 'r';
		maxhealth = 8;
		strength = 1;
		armor = 0;
		walk_rate = 1;
		follow_time = 10;
		luck = 45;
		verbs = {"bit"};
	}
	else if(species == "frog")
	{
		name = "Frog";
		tile = 'f';
		maxhealth = 5;
		strength = 0;
		armor = 0;
		walk_rate = 6;
		follow_time = -1;
		luck = 0;
	}
	else if(species == "troll")
	{
		name = "Troll";
		tile = 't';
		maxhealth = 50;
		strength = 12;
		armor = 12;
		walk_rate = 4; 
		follow_time = 24;
		luck = 75;
		verbs = {"slammed", "headbutted", "hit"};
	}
	else if(species == "witch")
	{
		name = "Witch";
		tile = 'w';
		maxhealth = 15;
		strength = 10;
		armor = 1;
		walk_rate = 3;
		follow_time = 27;
		luck = 80;
		verbs = {"struck"};
	}

	attack_rate = walk_rate;
	health = maxhealth;
	walk_counter = 0;
}

// Get attributes
int Mob::getWalkRate() { return walk_rate; }

std::pair <int, int> Mob::getPos() { return {y, x}; }

char Mob::getTile() { return tile; }

std::string Mob::getName() { return name; }

std::pair <int, int> Mob::getHealth() { return {health, maxhealth}; }

int Mob::getFollowTime() { return follow_time; }

int Mob::getLuck() { return luck; }

int Mob::getStrength() { return strength; }

int Mob::getArmor() { return armor; }

int Mob::distFrom(int qy, int qx)
{
	int dy = abs(y - qy);
	int dx = abs(x - qx);
	return dx + dy;
}

// Actions
void Mob::setSpawn(int py, int px)
{
	y = py;
	x = px;
	map[y][x].occupied = true;
}

void Mob::walk(std::pair <int, int> step)
{
	Tile checked = map[y + step.first][x + step.second];
	if(checked.tile == zero || checked.tile == wall || checked.tile == pit)
		return;
	if(checked.tile == water || checked.tile == door || checked.occupied)
		return;
	map[y][x].occupied = false;
	y += step.first;
	x += step.second;
	map[y][x].occupied = true;

}

bool Mob::seesPlayer(Character rogue)
{
	// Check if a mob can see the player (actually the oposite but who cares)
	return map[y][x].inView;
}


void Mob::findPath(const int fy, const int fx)
{
	// Clear old path first
	while(!path_to_player.empty())
		path_to_player.pop();

	const int inf = 1 << 30;
	// Using BFS
	bool been[300][300];
	std::pair <int, int> father[300][300];
	std::queue < std::pair <int, int> > q;
	std::pair <int, int> dir[4] = { {-1, 0}, {0, 1}, {0, -1}, {1, 0} };

	for(int i = 0; i < 300; i++)
	{
		for(int j = 0; j < 300; j++)
		{
			been[i][j] = 0;
			father[i][j] = {-inf, -inf};
		}
	}

	// fy, fx - target coordinates
	// cy, cx - current coordinates
	int cy, cx;
	father[this->y][this->x] = {-1, -1};
	been[this->y][this->x] = true;
	q.push({this->y, this->x});

	bool found = false;

	while(!q.empty() && !found)
	{
		cy = q.front().first;
		cx = q.front().second;
		q.pop();

		// Optional: more random than an L-shaped path
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(dir, dir + 4, std::default_random_engine(seed));

		for(int i = 0; i < 4; i++)
		{
			int yy = cy + dir[i].first;
			int xx = cx + dir[i].second;
			// Check if map is in level bounds
			if(yy < 0 || yy > maxy - 1 || xx < 0 || xx > maxx - 1)
				continue;
			// Check if map is walkable
			if(map[yy][xx].tile != path && map[yy][xx].tile != grass && map[yy][xx].tile != edge && map[yy][xx].tile != coast && map[yy][xx].tile != bridge)
				continue;
			// Check if map wasn't already processed
			if(!been[yy][xx])
			{
				been[yy][xx] = true;
				father[yy][xx] = {cy, cx};
				q.push({yy, xx});
			}
			if(yy == fy && xx == fx)
			{	
				found = true;
				break;
			}

		}
	}
	if(!found)
		return;
	//Get the path
	cy = fy;
	cx = fx;
	int tempy, tempx;
	
	path_to_player.push({fy, fx});
	while(true)
	{
		tempy = cy;
		tempx = cx;
		cy = father[tempy][tempx].first;
		cx = father[tempy][tempx].second;
		if(cy == -1 && cx == -1)
			break;
		path_to_player.push({cy, cx});
	}
	path_to_player.pop();
	return;
}

std::pair <int, int> Mob::getNextStep()
{
	// If mob is passive
	if(follow_time == -1)
	{
		int first = 0, second = 0;
		
		if(rand() % 2)
			first = (rand() % 2) ? 1 : -1;
		else
			second = ( rand() % 2 ) ? 1 : -1; 
		
		return {first, second};
	}
	if(path_to_player.empty())
		return {0, 0};
	std::pair <int, int> step = path_to_player.top();
	step.first -= y;
	step.second -= x;
	path_to_player.pop();
	return step;
}

void Mob::clearPath()
{
	while(!path_to_player.empty())
		path_to_player.pop();
	return;
}

int Mob::attack()
{
	// TODO
	// Make an actuall attack calculator shit
	if(rand() % 100 < getLuck() )
	{
		int damage = getStrength();
		rogue.health -= damage;
		return damage;
	}
	else
	{
		return 0;
	}
}

std::string Mob::getVerb() { return verbs[rand() % verbs.size()]; }
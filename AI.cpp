//
// File: AI.cpp
// Author: James Beller
// Group: Hack-'n-Slash
// Date: 12/6/2015
//
#include "AI.h"

#define INF 999999999

//
// The function that returns a random number between 2 given numbers.
//
// @Return - The random number between RandomAmount1 and RandomAmount2
// @Param RandomAmount1 - Lower range of the random number
// @Param RandomAmount2 - Upper range of the random number
//
int randomize(int RandomAmount1, int RandomAmount2)
{
	std::random_device rd;
	int Number = rd() % (RandomAmount2 - RandomAmount1) + RandomAmount1;

	return Number;
}
//
// The function that searches a given vector of pointers to see if any one of them point to
// a PathNode with the specified coordinates.
//
// @Return - True if there's a pointer to the PathNode with the specified coordinates, false otherwise
// @Param x - Specified x coordinate
// @Param y - Specified y coordinate
// @Param vect - Vector to check
//
bool InVector(int x, int y, std::vector<PathNode*> vect)
{
	for (std::vector<PathNode*>::iterator it = vect.begin(); it != vect.end(); it++)
	{
		if ((*it)->X() == x && (*it)->Y() == y)
			return true;
	}
	return false;
}
//
// The AI Constructor
//
// @Param ev_queue - Pointer to the event queue
// @Param SpriteImage - Pointer to the bitmap image to be used as the AI's sprite
// @Param t - Specified AI type
// @Param si - Specified AI sight
// @Param sp - Specified AI speed
// @Param he - Specified AI health
// @Param AK - Specified AI ATK
//
AI::AI(ALLEGRO_EVENT_QUEUE *ev_queue, ALLEGRO_BITMAP *SpriteImage, AI_TYPE t, int si, int sp, int he, int AK)
	: ai_ev_queue(ev_queue), state(IDLE), type(t), sight(si), speed(sp), health(he), ATK(AK), range(2), proj_active(false), melee_draw(false),
	tick_delay(TICK_DELAY_MAX), tick_hit_delay(HIT_DELAY), ai_x(0), ai_y(0), bound_x(48), bound_y(64), ai_direction(S), ai_dungeon(NULL),
	ai_tile(SpriteImage, 0, 0, bound_x, bound_y, true, true, false, true, 6), ai_melee_tile(0, 0, 48, 64, true, true, false, true, 6)
{
	// Register ai_boss_event_killed for bosses
	if (type == BOSS_MELEE || type == BOSS_RANGER)
	{
		al_init_user_event_source(&ai_boss_event_killed);
		al_register_event_source(ai_ev_queue, &ai_boss_event_killed);
		special_tick_delay = 0;
		buff_active = false;
	}
	// Register ai_event_killed for regular AI
	else
	{
		al_init_user_event_source(&ai_event_killed);
		al_register_event_source(ai_ev_queue, &ai_event_killed);
	}

	// Construct a projectile object for rangers
	if (type == RANGER || type == BOSS_RANGER)
	{
		int m_ProjectileXBound = 16;
		int m_ProjectileYBound = 16;
		int m_ProjectileSpeed = 12;
		ai_projectile = new Projectile(m_ProjectileXBound, m_ProjectileYBound, m_ProjectileSpeed, 0, 0, 0, -1);
		ai_projectile->ResetProjectile();
	}
	else
		ai_projectile = NULL;
}
//
// The function that checks to see if any of the AI's x bound points are within the player's
// bound area.
//
// @Return - True if v1 and v2 are in bound, false otherwise
// @Param v1 - Player's bound point edge
// @Param v2 - Player's bound point edge
//
bool AI::InBoundX(float v1, float v2)
{
	int b_x = GetXNorthBoundPoint();
	if (b_x >= v1 && b_x <= v2)
		return true;

	b_x = GetXSouthBoundPoint();
	if (b_x >= v1 && b_x <= v2)
		return true;

	b_x = GetXWestBoundPoint();
	if (b_x >= v1 && b_x <= v2)
		return true;

	b_x = GetXEastBoundPoint();
	if (b_x >= v1 && b_x <= v2)
		return true;

	return false;
}
//
// The function that checks to see if any of the AI's y bound points are within the player's
// bound area.
//
// @Return - True if v1 and v2 are in bound, false otherwise
// @Param v1 - Player's bound point edge
// @Param v2 - Player's bound point edge
//
bool AI::InBoundY(float v1, float v2)
{
	int b_y = GetYNorthBoundPoint();
	if (b_y >= v1 && b_y <= v2)
		return true;

	b_y = GetYSouthBoundPoint();
	if (b_y >= v1 && b_y <= v2)
		return true;

	b_y = GetYWestBoundPoint();
	if (b_y >= v1 && b_y <= v2)
		return true;

	b_y = GetYEastBoundPoint();
	if (b_y >= v1 && b_y <= v2)
		return true;

	return false;
}
//
// The function that checks to see if any of the projectile's x bound points are within the player's
// bound area.
//
// @Return - True if v1 and v2 are in bound, false otherwise
// @Param v1 - Player's bound point edge
// @Param v2 - Player's bound point edge
//
bool AI::ProjectileInBoundX(float v1, float v2)
{
	int proj_x = ai_projectile->GetHitBoxXBoundOne();
	if (proj_x >= v1 && proj_x <= v2)
		return true;

	proj_x = ai_projectile->GetHitBoxXBoundTwo();
	if (proj_x >= v1 && proj_x <= v2)
		return true;

	return false;
}
//
// The function that checks to see if any of the projectile's y bound points are within the player's
// bound area.
//
// @Return - True if v1 and v2 are in bound, false otherwise
// @Param v1 - Player's bound point edge
// @Param v2 - Player's bound point edge
//
bool AI::ProjectileInBoundY(float v1, float v2)
{
	int proj_y = ai_projectile->GetHitBoxYBoundOne();
	if (proj_y >= v1 && proj_y <= v2)
		return true;

	proj_y = ai_projectile->GetHitBoxYBoundTwo();
	if (proj_y >= v1 && proj_y <= v2)
		return true;

	return false;
}
//
// This function randomly sets the AI's spawn point in the specified dungeon.
// This function MUST be executed first before using the AI since this function
// sets the AI's dungeon pointer.
//
// @Param dungeon - The dungeon where the AI will spawn
//
void AI::SetSpawn(DungeonGenerator &dungeon)
{
	const int D_WIDTH = 51;         // Dungeon width
	const int D_HEIGHT = 31;        // Dungeon height

	std::vector<Rect> TempRooms;
	std::list<Rect> m_Rooms = dungeon.Get_Rooms();

	Rect TempRoom(D_WIDTH, D_HEIGHT, 1, 1);
	AVec2f PlayerStart = dungeon.GetStartPosition() / T_SIZE;

	// Get all rooms except the one with the player's starting position
	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		if (it->ContainsPoint(PlayerStart.x(), PlayerStart.y()))
			continue;
		TempRooms.push_back(*it);
	}

	TempRoom = TempRooms[randomize(0, TempRooms.size())];

	ai_x = randomize(TempRoom.Get_X1() + 1, TempRoom.Get_X2() - 1) * T_SIZE;
	ai_y = randomize(TempRoom.Get_Y1() + 1, TempRoom.Get_Y2() - 1) * T_SIZE;
	ai_dungeon = &dungeon;
}
//
// This function makes the AI move along the path to the target that has been found in AI::FindPath.
// The AI will move along the center of each tile.
//
void AI::MoveAlongPath()
{
	PathNode* p_node;

	// Do nothing if there's no path
	if (path.empty())
		return;
	else
	{
		// Get the PathNode from the back of the path vector
		p_node = path.back();
		// If the AI makes it to the middle of the tile specified by the current PathNode, pop it and get the next node
		if (ai_x == p_node->X() * T_SIZE + (T_SIZE / 2) && ai_y == p_node->Y() * T_SIZE + (T_SIZE / 2))
		{
			path.pop_back();
			// If the path vector becomes empty, the AI has reached the end of the path
			if (path.empty())
				return;
			p_node = path.back();
		}
		// Move towards the middle of the tile specified by the current PathNode
		MoveTowardTarget(p_node->X() * T_SIZE + (T_SIZE / 2), p_node->Y() * T_SIZE + (T_SIZE / 2));
	}
}
//
// This function makes the AI move toward the given coordinates.
//
// @Param t_x - Target x coordinate
// @Param t_y - Target y coordinate
//
void AI::MoveTowardTarget(int t_x, int t_y)
{
	if (ai_y > t_y)
	{
		MoveUp();
		// The nested if branches are useful whenever the AI's
		// speed makes the AI move right past the target coordinates
		if (ai_y < t_y)
			ai_y = t_y;
	}
	else if (ai_y < t_y)
	{
		MoveDown();
		if (ai_y > t_y)
			ai_y = t_y;
	}
	if (ai_x > t_x)
	{
		MoveLeft();
		if (ai_x < t_x)
			ai_x = t_x;
	}
	else if (ai_x < t_x)
	{
		MoveRight();
		if (ai_x > t_x)
			ai_x = t_x;
	}
}
//
// This function makes the AI move into a position where the player is in
// a straight line of the AI's sight and the AI is close enough.
//
// @Param p - The player in question
//
void AI::MoveIntoRange(Player &p)
{
	int p_x = p.GetXPosition();
	int p_y = p.GetYPosition();
	int ai_x_prev = ai_x;
	int ai_y_prev = ai_y;
	int dist_x = abs(ai_x - p_x);

	if (p_y != ai_y && dist_x > T_SIZE / 4)
	{
		MoveTowardTarget(ai_x, p_y);
		if (ai_x == ai_x_prev)
			MoveTowardTarget(p_x, ai_y);
	}
	else
	{
		MoveTowardTarget(p_x, ai_y);
		if (ai_y == ai_y_prev)
			MoveTowardTarget(ai_x, p_y);
	}
}
//
// This function makes the AI move up if it can. It also updates the facing direction and sprite.
//
void AI::MoveUp()
{
	ai_direction = N;
	if (!ai_dungeon->Get_Map()->CheckMapCollision(AVec2f(GetXNorthBoundPoint(), GetYNorthBoundPoint())))
	{
		ai_y -= speed;
		ai_tile.Set_CurRow(3, false);
		ai_tile.Event_Handler();
	}
}
//
// This function makes the AI move down if it can. It also updates the facing direction and sprite.
//
void AI::MoveDown()
{
	ai_direction = S;
	if (!ai_dungeon->Get_Map()->CheckMapCollision(AVec2f(GetXSouthBoundPoint(), GetYSouthBoundPoint())))
	{
		ai_y += speed;
		ai_tile.Set_CurRow(0, false);
		ai_tile.Event_Handler();
	}
}
//
// This function makes the AI move left if it can. It also updates the facing direction and sprite.
//
void AI::MoveLeft()
{
	ai_direction = W;
	if (!ai_dungeon->Get_Map()->CheckMapCollision(AVec2f(GetXWestBoundPoint(), GetYWestBoundPoint())))
	{
		ai_x -= speed;
		ai_tile.Set_CurRow(1, false);
		ai_tile.Event_Handler();
	}
}
//
// This function makes the AI move right if it can. It also updates the facing direction and sprite.
//
void AI::MoveRight()
{
	ai_direction = E;
	if (!ai_dungeon->Get_Map()->CheckMapCollision(AVec2f(GetXEastBoundPoint(), GetYEastBoundPoint())))
	{
		ai_x += speed;
		ai_tile.Set_CurRow(2, false);
		ai_tile.Event_Handler();
	}
}
//
// This function checks to see if the AI can see the player. It searches
// the tiles around the AI and within sight.
//
// @Return - True if the AI sees the player, false otherwise
// @Param p - The player in question
//
bool AI::SeePlayer(Player &p)
{
	int dai_x = ai_x / T_SIZE;            // AI x position relative to the dungeon
	int dai_y = ai_y / T_SIZE;            // AI y position relative to the dungeon
	int p_x = p.GetXPosition() / T_SIZE;  // Player x position relative to the dungeon
	int p_y = p.GetYPosition() / T_SIZE;  // Player y position relative to the dungeon
	// Search left
	for (int i = dai_x; i > (dai_x - sight); i--)
	{
		// The AI can't see through walls
		if (ai_dungeon->Get_Tile(AVec2i(i, dai_y)) == Wall)
			break;
		// Search up
		for (int j = dai_y; j > (dai_y - sight); j--)
		{
			if (i == p_x && j == p_y)
				return true;
			else if (ai_dungeon->Get_Tile(AVec2i(i, j)) == Wall)
				break;
		}
		// Search down
		for (int j = dai_y; j < (dai_y + sight); j++)
		{
			if (i == p_x && j == p_y)
				return true;
			else if (ai_dungeon->Get_Tile(AVec2i(i, j)) == Wall)
				break;
		}
	}
	// Search right
	for (int i = dai_x; i < (dai_x + sight); i++)
	{
		if (ai_dungeon->Get_Tile(AVec2i(i, dai_y)) == Wall)
			break;
		// Search up
		for (int j = dai_y; j > (dai_y - sight); j--)
		{
			if (i == p_x && j == p_y)
				return true;
			else if (ai_dungeon->Get_Tile(AVec2i(i, j)) == Wall)
				break;
		}
		// Search down
		for (int j = dai_y; j < (dai_y + sight); j++)
		{
			if (i == p_x && j == p_y)
				return true;
			else if (ai_dungeon->Get_Tile(AVec2i(i, j)) == Wall)
				break;
		}
	}
	return false;
}
//
// This function checks to see if the AI is in a position where the player is in
// a straight line of the AI's sight and the AI is close enough.
//
// @Return - True if the player is within range, false otherwise
// @Param p - The player in question
//
bool AI::InRange(Player &p)
{
	int p_x = p.GetXPosition();
	int p_y = p.GetYPosition();

	if (p_x == ai_x)
	{
		// Check North
		if (p_y < ai_y && p_y >= ai_y - (range * T_SIZE))
			return true;
		// Check South
		if (p_y > ai_y && p_y <= ai_y + (range * T_SIZE))
			return true;
	}
	if (p_y == ai_y)
	{
		// Check West
		if (p_x < ai_x && p_x >= ai_x - (range * T_SIZE))
			return true;
		// Check East
		if (p_x > ai_x && p_x <= ai_x + (range * T_SIZE))
			return true;
	}

	return false;
}
//
// This function makes the AI find the shortest path from its current position to
// the specified coordinates. The said path will be stored in the AI's path stack.
// This function uses the A* Pathfinding algorithm.
//
// @Param t_x - Target x coordinate (relative to the dungeon)
// @Param t_y - Target y coordinate (relative to the dungeon)
//
void AI::FindPath(int t_x, int t_y)
{
	int cur_x = ai_x / T_SIZE;  // Current x position relative to the dungeon
	int cur_y = ai_y / T_SIZE;  // Current y position relative to the dungeon
	bool path_found = false;
	std::vector<PathNode*> open_vector;
	std::vector<PathNode*> closed_vector;
	std::vector<PathNode*>::reverse_iterator p_it;
	PathNode* cur_node = NULL;
	int f_lowest;
	int g_tentative;

	const int D_WIDTH = 51;         // Dungeon width
	const int D_HEIGHT = 31;        // Dungeon height
	int g_score[D_WIDTH][D_HEIGHT];
	int f_score[D_WIDTH][D_HEIGHT];

	// If a path exists already, clear it first
	if (!path.empty())
		CleanPath();

	// Initialize the 2D arrays
	for (int i = 0; i < D_WIDTH; i++)
	{
		for (int j = 0; j < D_HEIGHT; j++)
		{
			g_score[i][j] = INF;
			f_score[i][j] = INF;
		}
	}
	// NOTE: The H score for this algorithm is calculated with the "Manhattan distance method"
	g_score[cur_x][cur_y] = 0;
	f_score[cur_x][cur_y] = abs(t_x - cur_x) + abs(t_y - cur_y);  // F = G + H with G being 0 here
	open_vector.push_back(new PathNode(cur_x, cur_y, f_score[cur_x][cur_y]));  // Put the starting tile into the open vector

	while (!open_vector.empty())
	{
		f_lowest = INF;
		// Find the lowest f_score
		for (std::vector<PathNode*>::reverse_iterator it = open_vector.rbegin(); it != open_vector.rend(); it++)
		{
			if (f_lowest > (*it)->F())
				f_lowest = (*it)->F();
		}
		// Get the most recent element with the lowest f_score
		for (std::vector<PathNode*>::reverse_iterator it = open_vector.rbegin(); it != open_vector.rend(); it++)
		{
			if (f_lowest == (*it)->F())
			{
				cur_node = *it;
				p_it = it;
				break;
			}
		}
		// If the coordinates in the node match the target coordinates, the path has been found
		if (cur_node->X() == t_x && cur_node->Y() == t_y)
		{
			path_found = true;
			break;
		}
		// Remove the current node from the open vector
		// NOTE: the vector's erase function takes iterators, not reverse_iterators. "--(p_it.base())" works around that issue
		open_vector.erase(--(p_it.base()));
		closed_vector.push_back(cur_node);
		// Look at each neighbor tile
		for (int i = cur_node->X() - 1; i <= cur_node->X() + 1; i++)
		{
			for (int j = cur_node->Y() - 1; j <= cur_node->Y() + 1; j++)
			{
				// Ignore the ones that are either closed or are unwalkable
				if (InVector(i, j, closed_vector) || ai_dungeon->Get_Tile(AVec2i(i, j)) == Wall)
					continue;
				// Don't check diagonally
				else if (i == cur_node->X() - 1 && j == cur_node->Y() - 1)
					continue;
				else if (i == cur_node->X() - 1 && j == cur_node->Y() + 1)
					continue;
				else if (i == cur_node->X() + 1 && j == cur_node->Y() - 1)
					continue;
				else if (i == cur_node->X() + 1 && j == cur_node->Y() + 1)
					continue;
				g_tentative = g_score[cur_node->X()][cur_node->Y()] + (abs(i - cur_node->X()) + abs(j - cur_node->Y()));
				if (!InVector(i, j, open_vector) || g_tentative < g_score[i][j])
				{
					g_score[i][j] = g_tentative;
					f_score[i][j] = g_tentative + (abs(t_x - i) + abs(t_y - j));
					if (!InVector(i, j, open_vector))
						open_vector.push_back(new PathNode(i, j, f_score[i][j], cur_node));
				}
			}
		}
	}
	if (!path_found)
	{
		// If the AI fails to find a path to the target coordinates, just delete all nodes allocated.
		for (std::vector<PathNode*>::iterator it = open_vector.begin(); it != open_vector.end(); it++)
			delete *it;
		for (std::vector<PathNode*>::iterator it = closed_vector.begin(); it != closed_vector.end(); it++)
			delete *it;
		std::cout << "The AI has failed to find a path...\n";
	}
	else
	{
		// Form the path from the goal to the AI's starting position
		while (cur_node)
		{
			path.push_back(cur_node);
			cur_node = cur_node->Pa();
		}
		// Put all PathNode pointers into the garbage vector so that all allocated PathNodes can be deleted later
		for (std::vector<PathNode*>::iterator it = open_vector.begin(); it != open_vector.end(); it++)
			garbage.push_back(*it);
		for (std::vector<PathNode*>::iterator it = closed_vector.begin(); it != closed_vector.end(); it++)
			garbage.push_back(*it);
		std::cout << "The AI has formed a path to target...\n";
	}
}
//
// This function deallocates all PathNodes created in AI::FindPath and clears all vectors.
// Gotta prevent memory leaks.
//
void AI::CleanPath()
{
	for (std::vector<PathNode*>::iterator it = garbage.begin(); it != garbage.end(); it++)
		delete *it;
	garbage.clear();
	path.clear();
}
//
// This function checks to see if the AI collides with the player.
//
// @Return - True if the AI collides with the player, false otherwise
// @Param p - The player in question
//
bool AI::CollideWithPlayer(Player &p)
{
	// Note the order of arguments for the calls to the Inbound functions here.
	// North and west go in negative directions while south and east go in positive directions.
	// For the Inbound functions, the AI bound point has to be in between the player's bound point
	// and the player's position.

	if (InBoundX(p.GetNorthWestXBoundPoint(), p.GetXPosition())
		&& InBoundY(p.GetNorthWestYBoundPoint(), p.GetYPosition()))
		return true;
	else if (InBoundX(p.GetXPosition(), p.GetNorthEastXBoundPoint())
		&& InBoundY(p.GetNorthEastYBoundPoint(), p.GetYPosition()))
		return true;
	else if (InBoundX(p.GetSouthWestXBoundPoint(), p.GetXPosition())
		&& InBoundY(p.GetYPosition(), p.GetSouthWestYBoundPoint()))
		return true;
	else if (InBoundX(p.GetXPosition(), p.GetSouthEastXBoundPoint())
		&& InBoundY(p.GetYPosition(), p.GetSouthEastYBoundPoint()))
		return true;

	return false;
}
//
// This function checks to see if the AI's projectile collides with the player.
//
// @Return - True if the projectile collides with the player, false otherwise
// @Param p - The player in question
//
bool AI::ProjectileCollideWithPlayer(Player &p)
{
	if (ProjectileInBoundX(p.GetNorthWestXBoundPoint(), p.GetXPosition())
		&& ProjectileInBoundY(p.GetNorthWestYBoundPoint(), p.GetYPosition()))
		return true;
	else if (ProjectileInBoundX(p.GetXPosition(), p.GetNorthEastXBoundPoint())
		&& ProjectileInBoundY(p.GetNorthEastYBoundPoint(), p.GetYPosition()))
		return true;
	else if (ProjectileInBoundX(p.GetSouthWestXBoundPoint(), p.GetXPosition())
		&& ProjectileInBoundY(p.GetYPosition(), p.GetSouthWestYBoundPoint()))
		return true;
	else if (ProjectileInBoundX(p.GetXPosition(), p.GetSouthEastXBoundPoint())
		&& ProjectileInBoundY(p.GetYPosition(), p.GetSouthEastYBoundPoint()))
		return true;

	return false;
}
//
// This function shoots a projectile in the direction the AI is currently facing.
//
void AI::Shoot()
{
	if (!ai_projectile)
		return;

	proj_active = true;
	ai_projectile->ResetProjectile();

	if (ai_direction == N)
		ai_projectile->SendProjecile(ai_x, ai_y, 0, -1);
	else if (ai_direction == S)
		ai_projectile->SendProjecile(ai_x, ai_y, 0, 1);
	else if (ai_direction == E)
		ai_projectile->SendProjecile(ai_x, ai_y, -1, 0);
	else if (ai_direction == W)
		ai_projectile->SendProjecile(ai_x, ai_y, 1, 0);
	else
		ai_projectile->SendProjecile(ai_x, ai_y, 0, -1);
}
//
// This function makes the AI face toward the player.
//
// @Param p - The player in question
//
void AI::FacePlayer(Player &p)
{
	if (ai_y > p.GetYPosition())
	{
		ai_direction = N;
		ai_tile.Set_CurRow(3, false);
		ai_tile.Event_Handler();
	}
	else if (ai_y < p.GetYPosition())
	{
		ai_direction = S;
		ai_tile.Set_CurRow(0, false);
		ai_tile.Event_Handler();
	}
	if (ai_x > p.GetXPosition())
	{
		ai_direction = W;
		ai_tile.Set_CurRow(1, false);
		ai_tile.Event_Handler();
	}
	else if (ai_x < p.GetXPosition())
	{
		ai_direction = E;
		ai_tile.Set_CurRow(2, false);
		ai_tile.Event_Handler();
	}
}
//
// This function checks to see if the given player bound points are within the AI's collusion bound.
//
// @Return - True if the bound points are within the AI's collusion bound, false otherwise
// @Param pb_x - The player's x bound point
// @Param pb_y - The player's y bound point
//
bool AI::CollusionBlock(int pb_x, int pb_y)
{
	if (pb_x >= GetXWestBoundPoint() && pb_x <= GetXEastBoundPoint())
		if (pb_y >= GetYNorthBoundPoint() && pb_y <= GetYSouthBoundPoint())
			return true;

	return false;
}
//
// This function checks to see if the given weapon bound points are within the AI's collusion bound.
// Deals damage to the AI if it is. If the AI's health drops to 0 or below, it dies.
//
// @Param w_x - The weapons's x bound point
// @Param w_y - The weapons's y bound point
// @Param w_d - The damage the weapon deals
//
void AI::WeaponHit(int w_x, int w_y, int w_d)
{
	if (w_x >= GetXWestBoundPoint() && w_x <= GetXEastBoundPoint())
		if (w_y >= GetYNorthBoundPoint() && w_y <= GetYSouthBoundPoint())
		{
			// The tick_hit_delay prevents the AI from taking damage every tick of one attack
			// It gets incremented in ProcessAI
			if (tick_hit_delay >= HIT_DELAY)
			{
				tick_hit_delay = 0;
				TakeDamage(w_d);
				std::cout << "The AI took " << w_d << " damage...\n";
				if (health <= 0)
				{
					if (type == BOSS_MELEE || type == BOSS_RANGER)
					{
						std::cout << "...and is now dead. You defeated the boss!\n";
						ai_ev.user.type = CUSTOM_EVENT_ID(BOSS_KILLED_EVENT);
						// Record where the AI was killed so AIGroup can spawn a pickup
						// where it died
						ai_ev.user.data1 = ai_x;
						ai_ev.user.data2 = ai_y;
						al_emit_user_event(&ai_boss_event_killed, &ai_ev, NULL);
					}
					else
					{
						std::cout << "...and is now dead...\n";
						ai_ev.user.type = CUSTOM_EVENT_ID(AI_KILLED_EVENT);
						ai_ev.user.data1 = ai_x;
						ai_ev.user.data2 = ai_y;
						al_emit_user_event(&ai_event_killed, &ai_ev, NULL);
					}
					state = DEAD;
				}
			}
		}
}
//
// This function draws the AI on the screen.
//
void AI::Draw()
{	
	// Do not draw dead AI
	if (state == DEAD)
		return;

	// Draw the AI's sprite
	ai_tile.Draw((ai_x - bound_x / 2), (ai_y - bound_y / 2));

	// For Melee AI, draw the AI's melee sprite (a fireball)
	if (type == MELEE || type == BOSS_MELEE)
		if (melee_draw == true)
			DrawMelee();

	// Draw the AI's projectile
	if (ai_projectile && proj_active)
		ai_projectile->Draw();

    // For debugging purposes, draw magenta circles showing the path the AI created to a target position
	// The circles are located at the center of each tile
	for (std::vector<PathNode*>::reverse_iterator it = path.rbegin(); it != path.rend(); it++)
		al_draw_filled_circle((*it)->X() * T_SIZE + (T_SIZE / 2), (*it)->Y() * T_SIZE + (T_SIZE / 2), 5, al_map_rgb(255, 0, 255));
}
//
// This function draws the melee AI's weapon sprite (a fireball) in a particular direction.
//
void AI::DrawMelee()
{
	if (ai_direction == N)
		ai_melee_tile.Draw(ai_x, (ai_y - bound_y / 2), 0, -1, true);
	else if (ai_direction == S)
		ai_melee_tile.Draw(ai_x, (ai_y + bound_y / 2), 0, 1, true);
	else if (ai_direction == W)
		ai_melee_tile.Draw((ai_x - bound_x / 2), ai_y, -1, 0, true);
	else if (ai_direction == E)
		ai_melee_tile.Draw((ai_x + bound_x / 2), ai_y, 1, 0, true);
}
//
// This function is where the AI gets processed.
//
// @Param ev - Allegro event variable
// @Param player - The player the AI will deal with
//
void AI::ProcessAI(ALLEGRO_EVENT &ev, Player &player)
{
	tick_hit_delay++;

	if (ev.type != ALLEGRO_EVENT_TIMER)
		return;
	
	ai_ev = ev;  // Update ai_ev
	ProcessProjectile(player);

	// Do not process dead AI
	if (type == DEAD)
		return;
	
	// Process buffs for bosses
	if (type == BOSS_MELEE || type == BOSS_RANGER)
		ProcessBossBuff();

	// Idle AI do nothing but check to see if it can see the player
	// The state will change to CHASE when it sees the player
	if (state == IDLE)
	{
		if (SeePlayer(player))
		{
			state = CHASE;
			std::cout << "The AI sees you...\n";
		}
	}
	// In this state, the AI will chase the player. The melee AI will move toward
	// the player and then switch its state to ATTCK when it collides with the player.
	// The ranger AI will move into a position where it can shoot the player, and then
	// change its state to ATTACK. When it loses sight of the player, it will form
	// a path to the coordinates where the player was last seen and then change
	// its state to SEEK.
	else if (state == CHASE)
	{
		if (!SeePlayer(player))
		{
			state = SEEK;
			FindPath(l_x / T_SIZE, l_y / T_SIZE);
			std::cout << "The AI lost sight of you, and is now heading towards your last known location...\n";
			return;
		}
		// Record the player's current position while chasing
		l_x = player.GetXPosition();
		l_y = player.GetYPosition();
		if (type == MELEE || type == BOSS_MELEE)
		{
			if (CollideWithPlayer(player))
			{
				state = ATTACK;
				std::cout << "The Melee AI is attacking you...\n";
			}
			MoveTowardTarget(l_x, l_y);
		}
		else if (type == RANGER || type == BOSS_RANGER)
		{
			if (InRange(player))
			{
				state = ATTACK;
				std::cout << "The Ranger AI is attacking you...\n";
			}
			MoveIntoRange(player);
		}
	}
	// In this state, the AI will move along the path it formed after chasing the player.
	// When it happens to see the player while in this state, the AI will clear the path
	// and then switch its state back to CHASE. When the AI reaches the end of the path without
	// seeing the player, it will change its state to IDLE.
	else if (state == SEEK)
	{
		if (SeePlayer(player))
		{
			CleanPath();
			state = CHASE;
			std::cout << "The AI found you...\n";
		}
		else if (!path.empty())
			MoveAlongPath();
		else
		{
			state = IDLE;
			std::cout << "The AI is now IDLE again...\n";
		}
	}
	// This state is where the AI will execute the helper functions for attacking the player.
	// The state will get changed in those functions.
	else if (state == ATTACK)
	{
		if (type == MELEE || type == BOSS_MELEE)
			MeleeAttack(player);
		else if (type == RANGER || type == BOSS_RANGER)
			RangerAttack(player);
	}
}
//
// Bosses use a special tick delay variable to determine when to toggle
// certain buffs. This function takes care of that.
//
void AI::ProcessBossBuff()
{
	if (type == BOSS_MELEE)
	{
		// Activate buff
		if (!buff_active && special_tick_delay >= TICK_DELAY_SPECIAL_MAX)
		{
			EnableMeleeBossBuff(5);
			std::cout << "Boss buff activated!\n";
			buff_active = true;
			special_tick_delay = 0;
		}
		// Deactivate buff
		else if (buff_active && special_tick_delay >= TICK_DELAY_SPECIAL_MIN)
		{
			DisableMeleeBossBuff(5);
			std::cout << "Boss buff deactivated...\n";
			buff_active = false;
			special_tick_delay = 0;
		}
	}
	else if (type == BOSS_RANGER)
	{
		if (!buff_active && special_tick_delay >= TICK_DELAY_SPECIAL_MAX)
		{
			EnableRangerBossBuff();
			std::cout << "Boss buff activated!\n";
			buff_active = true;
			special_tick_delay = 0;
		}
		else if (buff_active && special_tick_delay >= TICK_DELAY_SPECIAL_MIN)
		{
			DisableRangerBossBuff();
			std::cout << "Boss buff deactivated...\n";
			buff_active = false;
			special_tick_delay = 0;
		}
	}
	special_tick_delay++;
}
//
// This function processes the projectile if the the projectile is active.
//
// @Param p - The player the projectile will deal with
//
void AI::ProcessProjectile(Player &p)
{
	if (!proj_active)
		return;

	ai_projectile->UpdatePosition();
	
	// Stop the projectile if it hits an obstacle
	if (ai_dungeon->Get_Map()->CheckMapCollision(AVec2f(ai_projectile->GetHitBoxXBoundOne(), ai_projectile->GetHitBoxYBoundOne()))
		|| ai_dungeon->Get_Map()->CheckMapCollision(AVec2f(ai_projectile->GetHitBoxXBoundTwo(), ai_projectile->GetHitBoxYBoundTwo())))
		proj_active = false;
	// Deal damage and stop the projectile if it hits the player
	else if (ProjectileCollideWithPlayer(p))
	{
		DealDamageToPlayer(p, ATK);
		std::cout << "A projectile dealt " << ATK << " damage to player...\n";
		proj_active = false;
	}
}
//
// This function makes the melee AI attack the player in close range.
//
// @Param p - The player the AI will deal with
//
void AI::MeleeAttack(Player &p)
{
	if (!CollideWithPlayer(p))
	{
		state = CHASE;
		melee_draw = false;
		std::cout << "The Melee AI is now chasing you again...\n";
		return;
	}
	FacePlayer(p);
	tick_delay++;
	// Only draw the fireball for half of TICK_DELAY_MAX
	if (tick_delay >= TICK_DELAY_MAX / 2)
		melee_draw = false;
	// The delays will keep the AI from attacking the player every tick
	if (tick_delay >= TICK_DELAY_MAX)
	{
		DealDamageToPlayer(p, ATK);
		melee_draw = true;
		std::cout << "Dealing " << ATK << " damage to player...\n";
		tick_delay = 0;
	}
}
//
// This function makes the ranger AI shoot at the player in range.
//
// @Param p - The player the AI will deal with
//
void AI::RangerAttack(Player &p)
{
	if (!InRange(p))
	{
		state = CHASE;
		std::cout << "The Ranger AI is getting in range again...\n";
		return;
	}
	FacePlayer(p);
	tick_delay++;
	if (tick_delay >= TICK_DELAY_MAX)
	{
		Shoot();
		tick_delay = 0;
	}
}
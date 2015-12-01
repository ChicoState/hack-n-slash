//
// File: AI.cpp
// Author: James Beller
// Group: Hack-'n-Slash
// Date: 11/30/2015
//
#include "AI.h"

#define INF 999999999  // This is used for the pathfinding algorithm

//
// Copied from DungeonGenerator.cpp. This will be useful for setting a random
// spawn point for the AI. Added seed_modify to quickly ensure values like
// the AI's spawn coordinates and ID are unique.
//
int randomize(int RandomAmount1, int RandomAmount2)
{
	std::random_device rd;
	int Number = rd() % (RandomAmount2 - RandomAmount1) + RandomAmount1;

	return Number;
}
//
// Searches a given vector of pointers to see if any one of them point to
// a PathNode with the specified coordinates.
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
// AI Constructor
//
AI::AI(ALLEGRO_EVENT_QUEUE *ev_queue, ALLEGRO_BITMAP *SpriteImage, AI_TYPE t, int si, int sp, int he, int AK)
	: ai_ev_queue(ev_queue), state(IDLE), type(t), sight(si), speed(sp), health(he), ATK(AK), range(2), proj_active(false),
	tick_delay(TICK_DELAY_MAX), ai_x(0), ai_y(0), bound_x(48), bound_y(64), ai_direction(S), drops(ev_queue), ai_dungeon(NULL),
	ai_tile(SpriteImage, 0, 0, bound_x, bound_y, true, true, false, true, 6)
{
	if (type == BOSS_MELEE || type == BOSS_RANGER)
	{
		al_init_user_event_source(&ai_boss_event_killed);
		al_register_event_source(ai_ev_queue, &ai_boss_event_killed);
	}
	else
	{
		al_init_user_event_source(&ai_event_killed);
		al_register_event_source(ai_ev_queue, &ai_event_killed);
	}

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
// Checks to see if any of the AI's x bound points are within the player's
// bound area. Returns true if it is, false otherwise.
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
// Checks to see if any of the AI's y bound points are within the player's
// bound area. Returns true if it is, false otherwise.
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
// Checks to see if any of the projectile's x bound points are within the player's
// bound area. Returns true if it is, false otherwise.
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
// Checks to see if any of the projectile's y bound points are within the player's
// bound area. Returns true if it is, false otherwise.
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
// Set a random spawn point in a given dungeon. Most of the code here is
// based off of the code in DungeonGenerator::SetStartPosition that sets
// the player's starting position in the dungeon. This function MUST be executed
// first before the AI can be used because the ai_dungeon pointer has to
// be set.
//
void AI::SetSpawn(DungeonGenerator &dungeon)
{
	const int D_WIDTH = 51;         // Dungeon width
	const int D_HEIGHT = 31;        // Dungeon height

	std::vector<Rect> TempRooms;
	std::list<Rect> m_Rooms = dungeon.Get_Rooms();

	Rect TempRoom(D_WIDTH, D_HEIGHT, 1, 1);
	Vec2f PlayerStart = dungeon.GetStartPosition() / T_SIZE;

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
// Move along the path to the target that has been found in AI::FindPath. The AI will move
// along the center of each tile.
//
void AI::MoveAlongPath()
{
	PathNode* p_node;

	if (path.empty())
		return;
	else
	{
		p_node = path.back();
		if (ai_x == p_node->X() * T_SIZE + (T_SIZE / 2) && ai_y == p_node->Y() * T_SIZE + (T_SIZE / 2))
		{
			path.pop_back();
			if (path.empty())
				return;
			p_node = path.back();
		}
		MoveTowardTarget(p_node->X() * T_SIZE + (T_SIZE / 2), p_node->Y() * T_SIZE + (T_SIZE / 2));
	}
}
//
// Move toward the given coordinates.
//
void AI::MoveTowardTarget(int t_x, int t_y)
{
	if (ai_y > t_y)
	{
		MoveUp();
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
void AI::MoveIntoRange(Player &p)
{
	int p_x = p.GetXPosition();
	int p_y = p.GetYPosition();
	int dist_x = abs(ai_x - p_x);
	int dist_y = abs(ai_y - p_y);

	if (p_y != ai_y && dist_x > T_SIZE / 4)
		MoveTowardTarget(ai_x, p_y);
	else
		MoveTowardTarget(p_x, ai_y);
}
void AI::MoveUp()
{
	ai_direction = N;
	if (!ai_dungeon->Get_Map()->CheckMapCollision(Vec2f(GetXNorthBoundPoint(), GetYNorthBoundPoint())))
	{
		ai_y -= speed;
		ai_tile.Set_CurRow(3, false);
		ai_tile.Event_Handler();
	}
}
void AI::MoveDown()
{
	ai_direction = S;
	if (!ai_dungeon->Get_Map()->CheckMapCollision(Vec2f(GetXSouthBoundPoint(), GetYSouthBoundPoint())))
	{
		ai_y += speed;
		ai_tile.Set_CurRow(0, false);
		ai_tile.Event_Handler();
	}
}
void AI::MoveLeft()
{
	ai_direction = W;
	if (!ai_dungeon->Get_Map()->CheckMapCollision(Vec2f(GetXWestBoundPoint(), GetYWestBoundPoint())))
	{
		ai_x -= speed;
		ai_tile.Set_CurRow(1, false);
		ai_tile.Event_Handler();
	}
}
void AI::MoveRight()
{
	ai_direction = E;
	if (!ai_dungeon->Get_Map()->CheckMapCollision(Vec2f(GetXEastBoundPoint(), GetYEastBoundPoint())))
	{
		ai_x += speed;
		ai_tile.Set_CurRow(2, false);
		ai_tile.Event_Handler();
	}
}
//
// Check to see if the AI can see the player. Returns true if it can, false otherwise.
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
		if (ai_dungeon->Get_Tile(Vec2i(i, dai_y)) == Wall)
			break;
		// Search up
		for (int j = dai_y; j > (dai_y - sight); j--)
		{
			if (i == p_x && j == p_y)
				return true;
			else if (ai_dungeon->Get_Tile(Vec2i(i, j)) == Wall)
				break;
		}
		// Search down
		for (int j = dai_y; j < (dai_y + sight); j++)
		{
			if (i == p_x && j == p_y)
				return true;
			else if (ai_dungeon->Get_Tile(Vec2i(i, j)) == Wall)
				break;
		}
	}
	// Search right
	for (int i = dai_x; i < (dai_x + sight); i++)
	{
		if (ai_dungeon->Get_Tile(Vec2i(i, dai_y)) == Wall)
			break;
		// Search up
		for (int j = dai_y; j > (dai_y - sight); j--)
		{
			if (i == p_x && j == p_y)
				return true;
			else if (ai_dungeon->Get_Tile(Vec2i(i, j)) == Wall)
				break;
		}
		// Search down
		for (int j = dai_y; j < (dai_y + sight); j++)
		{
			if (i == p_x && j == p_y)
				return true;
			else if (ai_dungeon->Get_Tile(Vec2i(i, j)) == Wall)
				break;
		}
	}
	return false;
}
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
// Using the A* pathfinding algorithm, the AI will find the shortest path from its
// current position to the specified coordinates. The said path will be stored in the AI's
// path stack.
//
void AI::FindPath(int t_x, int t_y)
{
	int cur_x = ai_x / T_SIZE;  // Current x position relative to the dungeon
	int cur_y = ai_y / T_SIZE;  // Current y position relative to the dungeon
	bool path_found = false;
	std::vector<PathNode*> open_vector;
	std::vector<PathNode*> closed_vector;
	std::vector<PathNode*>::reverse_iterator p_it;
	PathNode* cur_node;
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
				if (InVector(i, j, closed_vector) || ai_dungeon->Get_Tile(Vec2i(i, j)) == Wall)
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
// Deallocates all PathNodes created in AI::FindPath and clears all vectors. Gotta prevent memory leaks :P
//
void AI::CleanPath()
{
	for (std::vector<PathNode*>::iterator it = garbage.begin(); it != garbage.end(); it++)
		delete *it;
	garbage.clear();
	path.clear();
}
//
// Checks to see if the AI collides with the player. Returns true if it does, false otherwise.
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
// Checks to see if the AI's projectile collides with the player.
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
// Shoots a projectile in the direction the AI is facing.
//
void AI::Shoot()
{
	if (!ai_projectile)
		return;

	proj_active = true;
	ai_projectile->ResetProjectile();

	if (ai_direction == N)
	{
		ai_projectile->SendProjecile(ai_x, ai_y, 0, -1);
	}
	else if (ai_direction == S)
	{
		ai_projectile->SendProjecile(ai_x, ai_y, 0, 1);
	}
	else if (ai_direction == E)
	{
		ai_projectile->SendProjecile(ai_x, ai_y, -1, 0);
	}
	else if (ai_direction == W)
	{
		ai_projectile->SendProjecile(ai_x, ai_y, 1, 0);
	}
	else
	{
		ai_projectile->SendProjecile(ai_x, ai_y, 0, -1);
	}
}
//
// Makes the AI face toward the player.
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
// Checks to see if the given player bound points are within the AI's collusion bound.
// Returns true if they do, false otherwise.
//
bool AI::CollusionBlock(int pb_x, int pb_y)
{
	if (pb_x >= GetXWestBoundPoint() && pb_x <= GetXEastBoundPoint())
		if (pb_y >= GetYNorthBoundPoint() && pb_y <= GetYSouthBoundPoint())
			return true;

	return false;
}
//
// Checks to see if the given weapon bound points are within the AI's collusion bound.
// Deals damage to the AI if it is. If the AI's health drops to 0 or below, it dies.
//
void AI::WeaponHit(int w_x, int w_y, int w_d)
{
	if (w_x >= GetXWestBoundPoint() && w_x <= GetXEastBoundPoint())
		if (w_y >= GetYNorthBoundPoint() && w_y <= GetYSouthBoundPoint())
		{
			TakeDamage(w_d);
			std::cout << "The AI took " << w_d << " damage...\n";
			if (health <= 0)
			{
				if (type == BOSS_MELEE || type == BOSS_RANGER)
				{
					std::cout << "...and is now dead. You defeated the boss!\n";
					ai_ev.user.type = CUSTOM_EVENT_ID(BOSS_KILLED_EVENT);
					al_emit_user_event(&ai_boss_event_killed, &ai_ev, NULL);
				}
				else
				{
					std::cout << "...and is now dead...\n";
					ai_ev.user.type = CUSTOM_EVENT_ID(AI_KILLED_EVENT);
					al_emit_user_event(&ai_event_killed, &ai_ev, NULL);
				}
				drops.SpawnObjectRandom(Vec2i(ai_x, ai_y), 100);
				state = DEAD;
			}
		}
}
//
// Draw the AI to the screen.
//
void AI::Draw()
{
	// Draw the pickup drop (even when the AI is dead)
	drops.Draw();
	if (state == DEAD)
		return;

	// Draw the AI's sprite
	ai_tile.Draw((ai_x - bound_x / 2), (ai_y - bound_y / 2));

	// Draw the AI's projectile
	if (ai_projectile && proj_active)
		ai_projectile->Draw();

    // For debugging purposes, draw magenta circles showing the path the AI created to a target position
	// The circles are located at the center of each tile
	for (std::vector<PathNode*>::reverse_iterator it = path.rbegin(); it != path.rend(); it++)
		al_draw_filled_circle((*it)->X() * T_SIZE + (T_SIZE / 2), (*it)->Y() * T_SIZE + (T_SIZE / 2), 5, al_map_rgb(255, 0, 255));
}
//
// The event handler for events besides ALLEGRO_EVENT_TIMER
//
void AI::EventHandler(ALLEGRO_EVENT &ev)
{
	drops.Event_Handler(ev);
}
//
// The AI in action.
//
void AI::ProcessAI(ALLEGRO_EVENT &ev, Player &player)
{
	if (ev.type != ALLEGRO_EVENT_TIMER)
		return;
	
	ai_ev = ev;
	ProcessProjectile(player);

	if (state == IDLE)
	{
		if (SeePlayer(player))
		{
			state = CHASE;
			std::cout << "The AI sees you...\n";
		}
	}
	else if (state == CHASE)
	{
		if (!SeePlayer(player))
		{
			state = SEEK;
			FindPath(l_x / T_SIZE, l_y / T_SIZE);
			std::cout << "The AI lost sight of you, and is now heading towards your last known location...\n";
			return;
		}
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
	else if (state == ATTACK)
	{
		if (type == MELEE || type == BOSS_MELEE)
			MeleeAttack(player);
		else if (type == RANGER || type == BOSS_RANGER)
			RangerAttack(player);
	}
}
//
// Process the projectile if the the projectile is active.
//
void AI::ProcessProjectile(Player &p)
{
	if (!proj_active)
		return;

	ai_projectile->UpdatePosition();
	
	// Stop the projectile if it hits an obstacle
	if (ai_dungeon->Get_Map()->CheckMapCollision(Vec2f(ai_projectile->GetHitBoxXBoundOne(), ai_projectile->GetHitBoxYBoundOne()))
		|| ai_dungeon->Get_Map()->CheckMapCollision(Vec2f(ai_projectile->GetHitBoxXBoundTwo(), ai_projectile->GetHitBoxYBoundTwo())))
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
// For melee types, attack the player in close range.
//
void AI::MeleeAttack(Player &p)
{
	if (!CollideWithPlayer(p))
	{
		state = CHASE;
		std::cout << "The Melee AI is now chasing you again...\n";
	}
	FacePlayer(p);
	if (tick_delay >= TICK_DELAY_MAX)
	{
		DealDamageToPlayer(p, ATK);
		std::cout << "Dealing " << ATK << " damage to player...\n";
		tick_delay = 0;
	}
	tick_delay++;
}
//
// For ranger types, shoot at the player in sight.
//
void AI::RangerAttack(Player &p)
{
	if (!InRange(p))
	{
		state = CHASE;
		std::cout << "The Ranger AI is getting in range again...\n";
	}
	FacePlayer(p);
	tick_delay++;
	if (tick_delay >= TICK_DELAY_MAX)
	{
		Shoot();
		tick_delay = 0;
	}
}
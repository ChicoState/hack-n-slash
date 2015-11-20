//
// File: AI.cpp
// Author: James Beller
// Group: Hack-'n-Slash
// Date: 11/16/2015
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
AI::AI(ALLEGRO_EVENT_QUEUE *ev_queue, ALLEGRO_BITMAP *SpriteImage, AI_TYPE t, int si, int sp)
	: ai_ev_queue(ev_queue), state(IDLE), type(t), sight(si), speed(sp), health(100), ATK(5),
	tick_delay(TICK_DELAY_MAX), ai_x(0), ai_y(0), bound_x(48), bound_y(64), ai_direction(N), ai_dungeon(NULL),
	ai_tile(SpriteImage, 0, 0, bound_x, bound_y, true, true, false, true, 6)
{
	al_init_user_event_source(&ai_event_killed);
	al_register_event_source(ai_ev_queue, &ai_event_killed);
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
// Set a random spawn point in a given dungeon. Most of the code here is
// based off of the code in DungeonGenerator::SetStartPosition that sets
// the player's starting position in the dungeon. This function MUST be executed
// first before the AI can be used because the ai_dungeon pointer has to
// be set.
//
void AI::SetSpawn(DungeonGenerator &dungeon)
{
	// NOTE: At the moment, the dungeon size (width and height) is constant.
	// This will change later
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

	// NOTE: At the moment, the dungeon size (width and height) is constant.
	// This will change later
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
// Makes the AI face toward the player.
//
void AI::FacePlayer(Player &p)
{
	if (ai_y > p.GetYPosition())
		ai_direction = N;
	else if (ai_y < p.GetYPosition())
		ai_direction = S;
	if (ai_x > p.GetXPosition())
		ai_direction = W;
	else if (ai_x < p.GetXPosition())
		ai_direction = E;
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
// Deals damage to the AI if it is.
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
				std::cout << "...and is now dead...\n";
				ai_ev.user.type = CUSTOM_EVENT_ID(AI_KILLED_EVENT);
				al_emit_user_event(&ai_event_killed, &ai_ev, NULL);
				state = DEAD;
			}
		}
}
//
// Draw the AI to the screen.
//
void AI::Draw()
{
	// A small red circle indicates where the AI is facing
	if (ai_direction == N)
		al_draw_filled_circle(ai_x, ai_y - (bound_y / 2), 2, al_map_rgb(255, 0, 0));
	else if (ai_direction == S)
		al_draw_filled_circle(ai_x, ai_y + (bound_y / 2), 2, al_map_rgb(255, 0, 0));
	else if (ai_direction == W)
		al_draw_filled_circle(ai_x - (bound_x / 2), ai_y, 2, al_map_rgb(255, 0, 0));
	else if (ai_direction == E)
		al_draw_filled_circle(ai_x + (bound_x / 2), ai_y, 2, al_map_rgb(255, 0, 0));

	// Draw the AI's sprite
	ai_tile.Draw((ai_x - bound_x / 2), (ai_y - bound_y / 2));

    // For debugging purposes, draw magenta circles showing the path the AI created to a target position
	// The circles are located at the center of each tile
	for (std::vector<PathNode*>::reverse_iterator it = path.rbegin(); it != path.rend(); it++)
		al_draw_filled_circle((*it)->X() * T_SIZE + (T_SIZE / 2), (*it)->Y() * T_SIZE + (T_SIZE / 2), 5, al_map_rgb(255, 0, 255));
}
//
// The AI in action.
//
void AI::ProcessAI(ALLEGRO_EVENT &ev, Player &player)
{
	ai_ev = ev;
	if (ai_ev.type != ALLEGRO_EVENT_TIMER)
		return;

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
		}
		else
		{
			l_x = player.GetXPosition();
			l_y = player.GetYPosition();
			if (CollideWithPlayer(player))
			{
				state = ATTACK;
				std::cout << "The AI is attacking you...\n";
			}
			MoveTowardTarget(l_x, l_y);
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
		if (!CollideWithPlayer(player))
		{
			state = CHASE;
			std::cout << "The AI is now chasing you again...\n";
		}
		FacePlayer(player);
		if (tick_delay >= TICK_DELAY_MAX)
		{
			DealDamageToPlayer(player, ATK);
			std::cout << "Dealing " << ATK << " damage to player...\n";
			tick_delay = 0;
		}
		tick_delay++;
	}
}

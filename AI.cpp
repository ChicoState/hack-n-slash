//
// File: AI.cpp
// Author: James Beller
// Group: Hack-'n-Slash
// Date: 10/29/2015
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
// Check to see if two given values are equal or close enough to be equal.
// This is for getting around an issue where the AI gets stuck while moving
// along the path with noneven speed values.
//
bool AI::ValueEqual(int p1, int p2)
{
	if (p1 == p2)
		return true;
	else if (p1 < p2 + speed && p1 > p2 - speed)
		return true;
	else
		return false;
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

	//Find the room furthest to the left of the screen
	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		if (it->Get_X1() < TempRoom.Get_X1())
		{
			TempRoom = *it;
		}
	}

	//Find any other rooms that may be at that Xposition
	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		if (it->Get_X1() == TempRoom.Get_X1())
		{
			TempRooms.push_back(*it);
		}
	}

	//Of all those rooms find the one that is the widest
	for (std::vector<Rect>::iterator it = TempRooms.begin(); it != TempRooms.end(); it++)
	{
		if (it->Get_X2() > TempRoom.Get_X2())
		{
			TempRoom = *it;
		}
	}

	//Find all rooms that are within the Xposition of the widest room
	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		if (it->Get_X1() < TempRoom.Get_X2())
		{
			TempRooms.push_back(*it);
		}
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
		if (ValueEqual(ai_x, p_node->X() * T_SIZE + (T_SIZE / 2)) && ValueEqual(ai_y, p_node->Y() * T_SIZE + (T_SIZE / 2)))
		{
			path.pop_back();
			if (path.empty())
				return;
			p_node = path.back();
		}
		if (ai_y > p_node->Y() * T_SIZE + (T_SIZE / 2))
			MoveUp();
		if (ai_y < p_node->Y() * T_SIZE + (T_SIZE / 2))
			MoveDown();
		if (ai_x > p_node->X() * T_SIZE + (T_SIZE / 2))
			MoveLeft();
		if (ai_x < p_node->X() * T_SIZE + (T_SIZE / 2))
			MoveRight();
	}
}
//
// Move toward the given coordinates.
//
void AI::MoveTowardTarget(int t_x, int t_y)
{
	if (ai_y > t_y)
		MoveUp();
	if (ai_y < t_y)
		MoveDown();
	if (ai_x > t_x)
		MoveLeft();
	if (ai_x < t_x)
		MoveRight();
}
void AI::MoveUp()
{
	if (ai_dungeon->Get_Tile(Vec2i(ai_x / T_SIZE, (ai_y - speed) / T_SIZE)) != Wall)
		ai_y -= speed;
}
void AI::MoveDown()
{
	if (ai_dungeon->Get_Tile(Vec2i(ai_x / T_SIZE, (ai_y + speed) / T_SIZE)) != Wall)
		ai_y += speed;
}
void AI::MoveLeft()
{
	if (ai_dungeon->Get_Tile(Vec2i((ai_x - speed) / T_SIZE, ai_y / T_SIZE)) != Wall)
		ai_x -= speed;
}
void AI::MoveRight()
{
	if (ai_dungeon->Get_Tile(Vec2i((ai_x + speed) / T_SIZE, ai_y / T_SIZE)) != Wall)
		ai_x += speed;
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
// Draw the AI to the screen.
//
void AI::Draw()
{
	// For now, the AI is represented by a magenta square
	al_draw_filled_rectangle(ai_x - (T_SIZE / 4), ai_y - (T_SIZE / 4),
		ai_x + (T_SIZE / 4), ai_y + (T_SIZE / 4), al_map_rgb(255, 0, 255));
	// A blue pixel represents the center of the square (where the AI coordinates are)
	al_draw_pixel(ai_x, ai_y, al_map_rgb(0, 0, 255));
    // For debugging purposes, draw magenta circles showing the path the AI created to a target position
	// The circles are located at the center of each tile
	for (std::vector<PathNode*>::reverse_iterator it = path.rbegin(); it != path.rend(); it++)
		al_draw_filled_circle((*it)->X() * T_SIZE + (T_SIZE / 2), (*it)->Y() * T_SIZE + (T_SIZE / 2), 5, al_map_rgb(255, 0, 255));
}
//
// The AI in action.
//
void AI::ProcessAI(Player &player)
{
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
}
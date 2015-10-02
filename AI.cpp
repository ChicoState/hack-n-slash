/**
 * File: AI.cpp
 * Author: James Beller
 * Group: Hack-'n-Slash
 * Date: 10/1/2015
 */
#include "AI.h"

#define INF 999999999

/**
 * Move one tile along the path to the target that has been found in AI::FindPath.
 * If there is no path, the AI will move in a random direction instead (calls AI::MoveRandom).
 */
void AI::MoveTowardTarget()
{
	PathNode* p_node;

	if (path.empty())
		MoveRandom();
	else
	{
		p_node = path.back();
		path.pop_back();
		SetXPosition(p_node->X());
		SetYPosition(p_node->Y());
	}
}
/**
 * Move one tile in a random direction. A random number is picked between 0 and 7. The
 * number determines the direction (can be N, NE, E, SE, S, SW, W, or NW).
 */
void AI::MoveRandom()
{
	int ai_x = ai_character->GetXPosition();
	int ai_y = ai_character->GetYPosition();
	srand(time(NULL));
	int n = rand() % 8;

	switch (n)
	{
	case 0:  // Move north
		ai_character->SetYPosition(ai_y - 1);
		break;
	case 1:  // Move northeast
		ai_character->SetXPosition(ai_x + 1);
		ai_character->SetYPosition(ai_y - 1);
		break;
	case 2:  // Move east
		ai_character->SetXPosition(ai_x + 1);
		break;
	case 3:  // Move southeast
		ai_character->SetXPosition(ai_x + 1);
		ai_character->SetYPosition(ai_y + 1);
		break;
	case 4:  // Move south
		ai_character->SetYPosition(ai_y + 1);
		break;
	case 5:  // Move southwest
		ai_character->SetXPosition(ai_x - 1);
		ai_character->SetYPosition(ai_y + 1);
		break;
	case 6:  // Move west
		ai_character->SetXPosition(ai_x - 1);
		break;
	case 7:  // Move northwest
		ai_character->SetXPosition(ai_x - 1);
		ai_character->SetYPosition(ai_y - 1);
		break;
	}
}
/**
 * Check to see if the AI can see the player. Returns true if it can, false otherwise.
 */
bool AI::SeePlayer(Player p, DungeonGenerator d)
{
	int ai_x = ai_character->GetXPosition();
	int ai_y = ai_character->GetYPosition();
	int p_x = p.GetXPosition();
	int p_y = p.GetYPosition();
	// Search left
	for (int i = ai_x; i > (ai_x - sight); i--)
	{
		// The AI can't see through walls
		if (d.GetTile(Vec2i(i, ai_y)) == Wall)
			break;
		// Search up
		for (int j = ai_y; j > (ai_y - sight); j--)
		{
			if (i == p_x && j == p_y)
				return true;
			else if (d.GetTile(Vec2i(i, j)) == Wall)
				break;
		}
		// Search down
		for (int j = ai_y; j < (ai_y + sight); j++)
		{
			if (i == p_x && j == p_y)
				return true;
			else if (d.GetTile(Vec2i(i, j)) == Wall)
				break;
		}
	}
	// Search right
	for (int i = ai_x; i < (ai_x + sight); i++)
	{
		if (d.GetTile(Vec2i(i, ai_y)) == Wall)
			break;
		// Search up
		for (int j = ai_y; j > (ai_y - sight); j--)
		{
			if (i == p_x && j == p_y)
				return true;
			else if (d.GetTile(Vec2i(i, j)) == Wall)
				break;
		}
		// Search down
		for (int j = ai_y; j < (ai_y + sight); j++)
		{
			if (i == p_x && j == p_y)
				return true;
			else if (d.GetTile(Vec2i(i, j)) == Wall)
				break;
		}
	}
	return false;
}
/**
 * Searches a given vector of pointers to see if any one of them point to
 * a PathNode with the specified coordinates.
 */
bool AI::InVector(int x, int y, std::vector<PathNode*> vect)
{
	for (std::vector<PathNode*>::iterator it = vect.begin(); it != vect.end(); it++)
	{
		if ((*it)->X() == x && (*it)->Y() == y)
			return true;
	}
	return false;
}
/**
 * Using the A* pathfinding algorithm, the AI will find the shortest path from its
 * current position to the specified coordinates. The said path will be stored in the AI's
 * path stack.
 */
void AI::FindPath(int t_x, int t_y, DungeonGenerator d)
{
	int ai_x = ai_character->GetXPosition();
	int ai_y = ai_character->GetYPosition();
	int cur_x = ai_x;
	int cur_y = ai_y;
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
	g_score[ai_x][ai_y] = 0;
	f_score[ai_x][ai_y] = abs(t_x - ai_x) + abs(t_y - ai_y);  // F = G + H with G being 0 here
	open_vector.push_back(new PathNode(ai_x, ai_y, f_score[ai_x][ai_y]));  // Put the starting tile into the open vector

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
				if (InVector(i, j, closed_vector) || d.GetTile(Vec2i(i, j)) == Wall)
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
/**
 * Deallocates all PathNodes created in AI::FindPath and clears all vectors. Gotta prevent memory leaks :P
 */
void AI::CleanPath()
{
	for (std::vector<PathNode*>::iterator it = garbage.begin(); it != garbage.end(); it++)
		delete *it;
	garbage.clear();
	path.clear();
}
/**
 * The AI in action.
 */
void AI::ProcessAI(Player player, DungeonGenerator dungeon)
{
	if (state == IDLE)
	{
		if (SeePlayer(player, dungeon))
		{
			state = CHASE;
			std::cout << "The AI sees you...\n";
		}
	}
	else if (state == CHASE)
	{
		if (!SeePlayer(player, dungeon))
		{
			state = IDLE;
			std::cout << "The AI lost sight of you...\n";
		}
	}
	else if (state == SEEK)
	{
		if (SeePlayer(player, dungeon))
		{
			CleanPath();
			state = CHASE;
			std::cout << "The AI found you...\n";
		}
		else if (!path.empty())
			MoveTowardTarget();
		else
		{
			state = IDLE;
			std::cout << "The AI is now IDLE again...\n";
		}
	}
}
//
// File: AI.h
// Author: James Beller
// Group: Hack-'n-Slash
// Date: 10/29/2015
//
#ifndef __AI_H__
#define __AI_H__

#include <math.h>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <cstdlib>
#include <random>
#include "Player.h"
#include "Vec2.h"
#include "DungeonGenerator.h"

enum AI_STATE{ IDLE, CHASE, SEEK };
enum AI_TYPE{ MELEE };
const int T_SIZE = 128;

//
// This class is for pathfinding.
//
class PathNode
{
private:
	int n_x, n_y;
	int f_score;
	PathNode* parent;
public:
	PathNode(int x, int y, int f) : n_x(x), n_y(y), f_score(f), parent(0) {}
	PathNode(int x, int y, int f, PathNode* p) : n_x(x), n_y(y), f_score(f), parent(p) {}
	int X() { return n_x; }
	int Y() { return n_y; }
	int F() { return f_score; }
	PathNode* Pa() { return parent; }
};

//
// The main AI class.
//
class AI
{
private:
	int state;                                     // The AI's current behavior (IDLE, ATTACK, etc.)
	int sight;                                     // How far the AI can see (in number of tiles)
	int type;                                      // The type of AI (MELEE, RANGER, etc)
	int speed;                                     // The movement speed of the AI
	std::vector<PathNode*> path;                   // The current path for the AI to follow
	std::vector<PathNode*> garbage;                // Use for deallocating all PathNodes when the AI no longer needs the path
	float ai_x, ai_y;                              // The coordinates of the AI's position relative to the display
	float l_x, l_y;                                // The coordinates where the player was last seen
	DungeonGenerator *ai_dungeon;                  // Pointer to the dungeon the AI is spawned in
	bool ValueEqual(int, int);                     // Check to see if given values are equal or close enough to be equal
	void MoveAlongPath();                          // Move along a path, if it exists, created in AI::FindPath
	void MoveTowardTarget(int, int);               // Move toward the specifed coordinates
	void MoveUp();                                 // Move up
	void MoveDown();                               // Move down
	void MoveLeft();                               // Move left
	void MoveRight();                              // Move right
	void CleanPath();                              // Deallocates all PathNodes created in AI::FindPath (Takes out the garbage)
public:
	AI(int t, int si, int sp) 
		: state(IDLE), type(t), sight(si), speed(sp), ai_x(0), ai_y(0), ai_dungeon(NULL) {}
	~AI() { CleanPath(); }                         // Gotta prevent memory leaks :P
	int GetState() { return state; }
	void SetState(int s) { state = s; }
	int GetType() { return type; }
	float GetXPosition() { return ai_x; }
	float GetYPosition() { return ai_y; }
	void SetXPosition(float x) { ai_x = x; }
	void SetYPosition(float y) { ai_y = y; }
	void Draw();                                   // Draw the AI to the screen
	void SetSpawn(DungeonGenerator &);             // Set a random spawn point and then set the ai_dungeon pointer
	void FindPath(int, int);                       // Find the shortest path to the given target coordinates
	bool SeePlayer(Player &);                      // Check if the AI can see the player
	void ProcessAI(Player &);                      // Process the AI
};

#endif
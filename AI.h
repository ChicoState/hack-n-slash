/**
 * File: AI.h
 * Author: James Beller
 * Group: Hack-'n-Slash
 * Date: 10/1/2015
 */
#ifndef __AI_H__
#define __AI_H__

#include <math.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Player.h"
#include "Character.h"
#include "Vec2.h"
#include "DungeonGenerator.h"

enum AI_STATE{ IDLE, CHASE, SEEK };
enum AI_TYPE{ MELEE };

/**
* This class is for pathfinding.
*/
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

class AI
{
private:
	int state;                                       // The AI's current behavior (IDLE, ATTACK, etc.)
	int sight;                                       // How far the AI can see (in number of tiles)
	int type;                                        // The type of AI (MELEE, RANGER, etc)
	std::vector<PathNode*> path;                     // The current path for the AI to follow
	std::vector<PathNode*> garbage;                  // Use for deallocating all PathNodes when the AI no longer needs the path
	Character *ai_character;                         // The character the AI will control
	bool InVector(int, int, std::vector<PathNode*>); // Checks if the specified x and y coordinates are in the specified vector
public:
	AI(int t, int s, Character *p) : state(IDLE), type(t), sight(s), ai_character(p) {}
	~AI() { delete ai_character; CleanPath(); }      // Gotta prevent memory leaks :P
	int GetState() { return state; }
	void SetState(int s) { state = s; }
	int GetType() { return type; }
	int GetXPosition() { return ai_character->GetXPosition(); }
	int GetYPosition() { return ai_character->GetYPosition(); }
	void SetXPosition(int x) { ai_character->SetXPosition(x); }
	void SetYPosition(int y) { ai_character->SetYPosition(y); }
	void MoveTowardTarget();                     // Move along a path, if it exists, created in AI::FindPath.
	void MoveRandom();                           // Move in a random direction
	void FindPath(int, int, DungeonGenerator);   // Find the shortest path to the given target coordinates
	void CleanPath();                            // Deallocates all PathNodes created in AI::FindPath (Takes out the garbage)
	bool SeePlayer(Player, DungeonGenerator);    // Check if the AI can see the player
	void ProcessAI(Player, DungeonGenerator);    // Process the AI
};

#endif
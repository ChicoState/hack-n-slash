//
// File: AI.h
// Author: James Beller
// Group: Hack-'n-Slash
// Date: 12/8/2015
//
#ifndef __AI_H__
#define __AI_H__

#include <math.h>
#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
#include <random>

#include "Player.h"
#include "AITile.h"
#include "Vec2.h"
#include "DungeonGenerator.h"
#include "Projectile.h"
#include "TerrainGenerator.h"
#include "AIMeleeTile.h"

// Constants
#define T_SIZE 128
#define HIT_DELAY 60
#define TICK_DELAY_MAX 90
#define TICK_DELAY_SPECIAL_MIN 150
#define TICK_DELAY_SPECIAL_MAX 200

// Enums
enum AI_STATE{ IDLE, CHASE, SEEK, ATTACK, DEAD };
enum AI_TYPE{ MELEE, RANGER, BOSS_MELEE, BOSS_RANGER };
enum AI_FACE{ N, S, W, E };

//
// The PathNode class for the pathfinding algorithm
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
	//
	// Private variables
	//
	AI_STATE state;                                // The AI's current behavior (IDLE, ATTACK, etc.)
	AI_TYPE type;                                  // The type of AI (MELEE, RANGER, etc)
	AI_FACE ai_direction;                          // The AI's current facing direction
	bool proj_active;                              // Bool that tells whether the projectile is active or not
	bool delay_hit;                                // Bool for ensuring that the AI gets hit one at a time
	bool melee_draw;                               // Bool for determining when to draw the AIMeleeTile
	bool buff_active;                              // Special bool for the boss AI (For temporary buffs)
	int sight;                                     // How far the AI can see (in number of tiles)
	int health, ATK, speed, range;                 // The AI's attribute values (note: range is in number of tiles)
	int tick_delay, tick_hit_delay;                // For timing purposes
	int special_tick_delay;                        // For timing with bosses
	int bound_x, bound_y;                          // x and y bounds for the AI
	float ai_x, ai_y;                              // The coordinates of the AI's position relative to the display
	float l_x, l_y;                                // The coordinates where the player was last seen
	AITile ai_tile;                                // The AI sprite
	AIMeleeTile ai_melee_tile;                     // The sprite to be used when a melee AI attacks the player
	DungeonGenerator *ai_dungeon;                  // Pointer to the dungeon the AI is spawned in
	Projectile *ai_projectile;                     // Pointer to the projectile the ranger AI will fire
	std::vector<PathNode*> path;                   // The current path for the AI to follow
	std::vector<PathNode*> garbage;                // Use for deallocating all PathNodes when the AI no longer needs the path
	ALLEGRO_EVENT ai_ev;                           // Allegro event variable
	ALLEGRO_EVENT_SOURCE ai_event_killed, ai_boss_event_killed;  // Custom allegro event sources
	ALLEGRO_EVENT_QUEUE *ai_ev_queue;              // Allegro event queue
	//
	// Private functions
	//
	void ThrowAway(std::vector<PathNode*> &);      // Put PathNode pointers in a vector into the garbage vector
	bool InBoundX(float, float);                   // Check to see if an x bound point is within 2 given values
	bool InBoundY(float, float);                   // Check to see if a y bound point is within 2 given values
	bool ProjectileInBoundX(float, float);         // Check to see if the projectile's x bound is within 2 given values
	bool ProjectileInBoundY(float, float);         // Check to see if the projectile's y bound is within 2 given values
	bool InRange(Player &);                        // Check to see if the player is within firing range
	void MoveAlongPath();                          // Move along a path, if it exists, created in AI::FindPath
	void MoveTowardTarget(int, int);               // Move toward the specifed coordinates
	void MoveIntoRange(Player &);                  // Get into range
	void MoveUp();                                 // Move up
	void MoveDown();                               // Move down
	void MoveLeft();                               // Move left
	void MoveRight();                              // Move right
	void CleanPath();                              // Deallocates all PathNodes created in AI::FindPath (Takes out the garbage)
	void FacePlayer(Player &p);                    // Makes the AI face towards the player
	bool CollideWithPlayer(Player &p);             // Check to see if it the AI collides with the player
	bool ProjectileCollideWithPlayer(Player &);    // Check to see if the projectile collides with the player
	void Shoot();                                  // Makes the AI shoot a projectile
	// Helper function for FindPath
	void SearchNeighbors(PathNode*, int, int, int[][31], int[][31],
		std::vector<PathNode*> &, std::vector<PathNode*> &);  // Search the neighbors of a tile
	// Helper function for Draw
	void DrawMelee();                              // Draw the melee weapon sprite
	// Helper functions for SeePlayer
	bool SearchLeft(int, int, int, int);           // Search left for the player
	bool SearchRight(int, int, int, int);          // Search right for the player
	// Helper functions for ProcessAI
	void AIChase(Player &);
	void AISeek(Player &);
	void ProcessProjectile(Player &);              // Process the projectile (if it is active)
	void MeleeAttack(Player &);                    // Melee AI calls this function in ATTACK mode
	void RangerAttack(Player &);                   // Ranger AI calls this function in ATTACK mode
	void ProcessBossBuff();                        // Process buffs for the bosses (toggles buffs sometimes)
	// One line functions
	void DealDamageToPlayer(Player &p, int v) { p.DealDamage(v); }  // Deal melee damage to player
	void TakeDamage(int v) { health -= v; }                         // Take damage
	// Special functions for bosses
	void EnableMeleeBossBuff(int b) { speed += 5; ATK += b; }       // Boost the speed and ATK of melee bosses
	void DisableMeleeBossBuff(int b) { speed -= 5; ATK -= b; }      // Disable buff of melee bosses
	void EnableRangerBossBuff() { speed -= 1; ATK *= 2; }           // Penalize speed of ranger bosses, but double their ATK
	void DisableRangerBossBuff() { speed += 1; ATK /= 2; }          // Disable buff of ranger bosses
	// Other one line functions that return the bound points of the AI at particular locations
	int GetXNorthBoundPoint() { return ai_x; }
	int GetYNorthBoundPoint() { return (ai_y - (bound_y / 2)); }
	int GetXSouthBoundPoint() { return ai_x; }
	int GetYSouthBoundPoint() { return (ai_y + (bound_y / 2)); }
	int GetXEastBoundPoint() { return (ai_x + (bound_x / 2)); }
	int GetYEastBoundPoint() { return ai_y; }
	int GetXWestBoundPoint() { return (ai_x - (bound_x / 2)); }
	int GetYWestBoundPoint() { return ai_y; }
	int GetXNorthEastBoundPoint() { return (GetXNorthBoundPoint() + (bound_x / 2)); }
	int GetYNorthEastBoundPoint() { return (GetYNorthBoundPoint()); }
	int GetXNorthWestBoundPoint() { return (GetXNorthBoundPoint() - (bound_x / 2)); }
	int GetYNorthWestBoundPoint() { return (GetYNorthBoundPoint()); }
	int GetXSouthEastBoundPoint() { return (GetXSouthBoundPoint() + (bound_x / 2)); }
	int GetYSouthEastBoundPoint() { return (GetYSouthBoundPoint()); }
	int GetXSouthWestBoundPoint() { return (GetXSouthBoundPoint() - (bound_x / 2)); }
	int GetYSouthWestBoundPoint() { return (GetYSouthBoundPoint()); }
public:
	//
	// Public functions
	//
	AI(ALLEGRO_EVENT_QUEUE *ev_queue, ALLEGRO_BITMAP *SpriteImage, AI_TYPE t, int si, int sp, int he, int AK);
	~AI() { CleanPath(); delete ai_projectile; }   // Gotta prevent memory leaks :P
	bool CollusionBlock(int, int);                 // Detect collusion between player's bounds and the AI's bounds
	void WeaponHit(int, int, int);                 // Detect collusion between the weapon and the AI
	void Draw();                                   // Draw the AI to the screen
	void SetSpawn(DungeonGenerator &);             // Set a random spawn point and then set the ai_dungeon pointer
	void FindPath(int, int);                       // Find the shortest path to the given target coordinates (using A*)
	bool SeePlayer(Player &);                      // Check to see if the AI can see the player
	void ProcessAI(ALLEGRO_EVENT &, Player &);     // Process the AI
	int GetState() { return state; }               // Return the AI's current state
	int GetType() { return type; }                 // Return the AI's type
	float GetXPosition() { return ai_x; }          // Return the AI's x position
	float GetYPosition() { return ai_y; }          // Return the AI's y position
	void SetXPosition(float x) { ai_x = x; }       // Set the AI's x position
	void SetYPosition(float y) { ai_y = y; }       // Set the AI's y position
	DungeonGenerator* GetActiveDungeon() { return ai_dungeon; }  // Return the pointer to the dungeon the AI is in
};
#endif
//
// File: AI.h
// Author: James Beller
// Group: Hack-'n-Slash
// Date: 12/1/2015
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
#include "AITile.h"
#include "Vec2.h"
#include "DungeonGenerator.h"
#include "Projectile.h"
#include "TerrainGenerator.h"

#include "SwordWeapon.h"
#include "BowWeapon.h"

enum AI_STATE{ IDLE, CHASE, SEEK, ATTACK, DEAD };
enum AI_TYPE{ MELEE, RANGER, BOSS_MELEE, BOSS_RANGER };
enum AI_FACE{ N, S, W, E };

const int T_SIZE = 128;
const int TICK_DELAY_MAX = 90;
const int HIT_DELAY = 60;
// Special Tick Delays reserved for boss AI
const int TICK_DELAY_SPECIAL_MIN = 150;
const int TICK_DELAY_SPECIAL_MAX = 200;

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
	//
	// Private variables
	//
	AI_STATE state;                                // The AI's current behavior (IDLE, ATTACK, etc.)
	AI_TYPE type;                                  // The type of AI (MELEE, RANGER, etc)
	AI_FACE ai_direction;                          // The AI's current facing direction
	bool proj_active;                              // Is the projectile active?
	bool delay_hit;                                // Bool for ensuring that the AI gets hit one at a time
	bool buff_active;                              // Special bool reserved for the boss AI.
	int sight;                                     // How far the AI can see (in number of tiles)
	int health, ATK, speed, range;                 // The AI's attribute values (note: range is in number of tiles)
	int tick_delay, tick_hit_delay;                // For timing purposes
	int special_tick_delay;                        // For timing with bosses
	int bound_x, bound_y;                          // x and y bounds for the AI
	float ai_x, ai_y;                              // The coordinates of the AI's position relative to the display
	float l_x, l_y;                                // The coordinates where the player was last seen
	AITile ai_tile;                                // The AI sprite, using the PlayerTile class
	DungeonGenerator *ai_dungeon;                  // Pointer to the dungeon the AI is spawned in
	Projectile *ai_projectile;                     // The projectile the ranger AI will fire
	std::vector<PathNode*> path;                   // The current path for the AI to follow
	std::vector<PathNode*> garbage;                // Use for deallocating all PathNodes when the AI no longer needs the path
	TerrainObject_Manager drops;                   // Use for dropping pickups upon death
	ALLEGRO_EVENT ai_ev;
	ALLEGRO_EVENT_SOURCE ai_event_killed;
	ALLEGRO_EVENT_SOURCE ai_boss_event_killed;
	ALLEGRO_EVENT_QUEUE *ai_ev_queue;

	//
	// Private functions
	//
	bool InBoundX(float, float);                   // Check to see if an x bound point is within 2 given values
	bool InBoundY(float, float);                   // Check to see if a y bound point is within 2 given values
	bool ProjectileInBoundX(float, float);         // Check to see if the projectile's x bound is within 2 given values
	bool ProjectileInBoundY(float, float);         // Check to see if the projectile's y bound is within 2 given values
	bool InRange(Player &);
	void MoveAlongPath();                          // Move along a path, if it exists, created in AI::FindPath
	void MoveTowardTarget(int, int);               // Move toward the specifed coordinates
	void MoveIntoRange(Player &);                  // Get into range
	void MoveUp();                                 // Move up
	void MoveDown();                               // Move down
	void MoveLeft();                               // Move left
	void MoveRight();                              // Move right
	void CleanPath();                              // Deallocates all PathNodes created in AI::FindPath (Takes out the garbage)
	void FacePlayer(Player &p);                    // Makes the AI face towards the player
	bool CollideWithPlayer(Player &p);             // Checks to see if it the AI collides with the player
	bool ProjectileCollideWithPlayer(Player &);    // Checks to see if the projectile collides with the player
	void Shoot();                                  // Makes the AI shoot a projectile
	// Helper functions for ProcessAI
	void ProcessProjectile(Player &);
	void MeleeAttack(Player &);
	void RangerAttack(Player &);
	void ProcessBossBuff();
	// One line functions
	void DealDamageToPlayer(Player &p, int v) { p.DealDamage(v); }  // Deal melee damage to player
	void TakeDamage(int v) { health -= v; }
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
	void FindPath(int, int);                       // Find the shortest path to the given target coordinates
	bool SeePlayer(Player &);                      // Check if the AI can see the player
	void EventHandler(ALLEGRO_EVENT &);            // Event handler for the AI
	void ProcessAI(ALLEGRO_EVENT &, Player &);     // Process the AI
	int GetState() { return state; }
	int GetType() { return type; }
	float GetXPosition() { return ai_x; }
	float GetYPosition() { return ai_y; }
	void SetXPosition(float x) { ai_x = x; }
	void SetYPosition(float y) { ai_y = y; }
	DungeonGenerator* GetActiveDungeon() { return ai_dungeon; }
};
#endif
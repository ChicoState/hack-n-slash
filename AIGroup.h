//
// File: AIGroup.h
// Author: James Beller
// Group: Hack-'n-Slash
// Date: 12/4/2015
//
#ifndef __AIGROUP_H__
#define __AIGROUP_H__

#include <allegro5\allegro.h>

#include "AI.h"

//
// This class handles 1 or more AI instances in a container.
//
class AI_Group
{
private:
	//
	// Private variables
	//
	std::map<int,AI*> group;           // The container holding the group of AI instances. An ID is mapped with each AI.
	bool BossActive;                   // Bool that tells whether the boss is active or not
	int BossID;                        // ID of the active boss
	//
	// Private functions
	//
	bool IDExists(int);                // Check the container to see if the specified ID exists
	bool Overlap(AI*);                 // Check to see if an AI overlaps with any other AI in the group
	bool OverlapWithPlayerStart(AI*);  // Check to see if an AI overlaps with the player's starting position
	ALLEGRO_BITMAP *melee_sprite;      // Bitmap to be used for melee AI
	ALLEGRO_BITMAP *ranger_sprite;     // Bitmap to be used for ranger AI
	ALLEGRO_BITMAP *boss_melee_sprite; // Bitmap to be used for boss melee AI
	ALLEGRO_BITMAP *boss_ranger_sprite;// Bitmap to be used for boss ranger AI
	ALLEGRO_EVENT_QUEUE *e_queue;      // Pointer to event queue to use when adding AI
	DungeonGenerator *dungeon;         // Pointer to the dungeon the group will be in
public:
	//
	// Public functions
	//
	AI_Group(ALLEGRO_EVENT_QUEUE *);
	~AI_Group() { GroupClear(); }
	void EventHandler(ALLEGRO_EVENT &);                             // Event handler for the group
	void RandomSetup(int, DungeonGenerator &);                      // Set up the group with random attributes
    void AddRandom();                                               // Generate a new AI and add it to the group
	void SpawnBoss(int, int);                                       // Spawn a boss AI at specified coordinates
	void GroupClear();                                              // Deallocate all AI and clear the container
	void ProcessAll(ALLEGRO_EVENT &, Player &);                     // Process everyone in the group
	void DrawAll();                                                 // Draw everyone on the screen
	bool CollideWithAI(int, int);                                   // Check to see if the given bound points collide with any of the AI
	void HitAI(int, int, int);                                      // Check to see if the active weapon collides with any of the AI
};
#endif
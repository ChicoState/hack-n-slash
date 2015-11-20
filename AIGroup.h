//
// File: AIGroup.h
// Author: James Beller
// Group: Hack-'n-Slash
// Date: 11/16/2015
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
	std::map<int,AI*> group;           // The container holding the group of AI instances. An ID is mapped with each AI.
	bool IDExists(int);                // Checks the container to see if the specified ID exists
	bool Overlap(AI*);                 // Check to see if an AI overlaps with any other AI in the group
	bool OverlapWithPlayerStart(AI*);  // Check to see if an AI overlaps with the player's starting position
	ALLEGRO_EVENT_QUEUE *e_queue;
public:
	AI_Group(ALLEGRO_EVENT_QUEUE* e_q) : e_queue(e_q) {}
	~AI_Group() { GroupClear(); }
	void RandomSetup(int, DungeonGenerator &, ALLEGRO_BITMAP *);    // Set up the group with random attributes
    void AddRandom(DungeonGenerator &, ALLEGRO_BITMAP *);           // Generate a new AI and add it to the group
	void GroupClear();                                              // Deallocate all AI and clear the container
	void ProcessAll(ALLEGRO_EVENT &, Player &);                     // Process everyone in the group
	void DrawAll();                                                 // Draw everyone onto the screen
	void GetPathToPlayer(Player &);                                 // Make everyone find a path to the player (For testing purposes; may be removed later)
	bool CollideWithAI(int, int);                                   // Check to see if the given bound points collide with any of the AI
	void HitAI(int, int, int);                                      // Check to see if the active weapon collides with any of the AI
};
#endif

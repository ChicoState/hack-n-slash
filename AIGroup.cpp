//
// File: AIGroup.cpp
// Author: James Beller
// Group: Hack-'n-Slash
// Date: 10/29/2015
//
#include "AIGroup.h"

//
// Check the container to see if the specified ID exists in the container.
//
bool AI_Group::IDExists(int id)
{
	for (std::map<int, AI*>::iterator it = group.begin(); it != group.end(); it++)
	{
		if (id == it->first)
			return true;
	}
	return false;
}
//
// Check to see if the given AI overlaps with anyone else in the group
//
bool AI_Group::Overlap(AI* ai)
{
	for (std::map<int, AI*>::iterator it = group.begin(); it != group.end(); it++)
	{
		if ((it->second)->GetXPosition() == ai->GetXPosition()
			&& (it->second)->GetYPosition() == ai->GetYPosition())
			return true;
	}
	return false;
}
//
// Set up the group with a given number of randomly generated AI.
//
void AI_Group::RandomSetup(int n, DungeonGenerator &d)
{
	// If the group is not empty, clear it first
	if (!group.empty())
		GroupClear();

	for (int i = 0; i < n; i++)
		AddRandom(d);
}
//
// Generate a new AI with random attributes and then add it to the group.
//
void AI_Group::AddRandom(DungeonGenerator &d)
{
	AI* ai;
	std::random_device rd;
	int id, sight, speed;
 
	id = rd() % 99999 + 1;             // Get random ID value (up to 5 digits)
	while (IDExists(id))               // Ensure the ID is unique
		id = rd() % 99999 + 1;
	sight = rd() % 4 + 2;              // Sight ranges from 2 to 5  (This may change later)
	speed = rd() % 4 + 1;              // Speed ranges from 1 to 4  (This may change later)
	ai = new AI(MELEE, sight, speed);  // There's only one AI type for now
	ai->SetSpawn(d);                   // Set a spawn point
	while (Overlap(ai))                // Ensure the new AI doesn't overlap with anyone else in the group
		ai->SetSpawn(d);
	group.insert(std::pair<int, AI*>(id, ai));
}
//
// Deallocate all AI and clear the container.
//
void AI_Group::GroupClear()
{
	for (std::map<int, AI*>::iterator it = group.begin(); it != group.end(); it++)
		delete it->second;
	group.clear();
}
//
// Process all AI in the group.
//
void AI_Group::ProcessAll(Player &p)
{
	for (std::map<int, AI*>::iterator it = group.begin(); it != group.end(); it++)
		(it->second)->ProcessAI(p);
}
//
// Draw everyone in the group onto the screen.
//
void AI_Group::DrawAll()
{
	for (std::map<int, AI*>::iterator it = group.begin(); it != group.end(); it++)
		(it->second)->Draw();
}
//
// Make everyone in the group find a path to the player.
// This is here for testing purposes, and may be removed later.
//
void AI_Group::GetPathToPlayer(Player &p)
{
	for (std::map<int, AI*>::iterator it = group.begin(); it != group.end(); it++)
		(it->second)->FindPath(p.GetXPosition()/T_SIZE, p.GetYPosition()/T_SIZE);
}
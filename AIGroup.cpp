//
// File: AIGroup.cpp
// Author: James Beller
// Group: Hack-'n-Slash
// Date: 12/1/2015
//
#include "AI.h"
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
// Event handler
//
void AI_Group::EventHandler(ALLEGRO_EVENT &ev)
{
	if (ev.type == SPAWN_BOSS_EVENT && !BossActive)
	{
		ALLEGRO_BITMAP *image = al_load_bitmap("AI_Boss.png");
		SpawnBoss(image, ev.user.data1, ev.user.data2);
	}
	else if (ev.type == PLAYERPOSITION_EVENT)
	{
		for (std::map<int, AI*>::iterator it = group.begin(); it != group.end(); it++)
			(it->second)->EventHandler(ev);
	}
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
// Check to see if the given AI overlaps with the player's starting position
//
bool AI_Group::OverlapWithPlayerStart(AI* ai)
{
	Vec2f pos = ai->GetActiveDungeon()->GetStartPosition();

	if (ai->GetXPosition() == pos.x() && ai->GetYPosition() == pos.y())
		return true;
	else
		return false;
}
//
// Set up the group with a given number of randomly generated AI.
//
void AI_Group::RandomSetup(int n, DungeonGenerator &d, ALLEGRO_BITMAP *image)
{
	// If the group is not empty, clear it first
	if (!group.empty())
		GroupClear();

	BossActive = false;
	dungeon = &d;

	for (int i = 0; i < n; i++)
		AddRandom(image);
}
//
// Generate a new AI with random attributes and then add it to the group.
//
void AI_Group::AddRandom(ALLEGRO_BITMAP *image)
{
	AI* ai;
	std::random_device rd;
	int id, sight, speed, type, health, ATK;
	int lv = dungeon->Get_DungeonLevel();

	type = rd() % 2;                           // Randomly pick either 0 or 1 (0 = Melee, 1 = Ranger)
	id = rd() % 99999 + 1;                     // Get random ID value (up to 5 digits)
	while (IDExists(id))                       // Ensure the ID is unique
		id = rd() % 99999 + 1;
	sight = rd() % 3 + 3;                      // Sight ranges from 3 to 5
	speed = rd() % 3 + 3;                      // Speed ranges from 3 to 5
	health = rd() % 5 + 18 + (20 * (lv - 1));  // Health ranges from 18 to 22 (values get higher by 20 per level as dungeon level increases)
	ATK = rd() % 4 + 5 + (2 * (lv - 1));       // ATK ranges from 5 to 8 (values get higher by 2 per level as dungeon level increases)

	if (!type)
		ai = new AI(e_queue, image, MELEE, sight, speed, health, ATK);
	else
		ai = new AI(e_queue, image, RANGER, sight, speed, health, ATK);

	ai->SetSpawn(*dungeon);      // Set a spawn point
	while (Overlap(ai) || OverlapWithPlayerStart(ai))    // Ensure the new AI doesn't spawn on top of anyone else
		ai->SetSpawn(*dungeon);

	group.insert(std::pair<int, AI*>(id, ai));
}
//
// Generate a boss AI for the dungeon
//
void AI_Group::SpawnBoss(ALLEGRO_BITMAP *image, int x, int y)
{
	AI* ai;
	std::random_device rd;
	int sight, speed, type, health, ATK;
	int lv = dungeon->Get_DungeonLevel();
	BossActive = true;

	type = rd() % 2;                             // Randomly pick either 0 or 1 (0 = Melee, 1 = Ranger)
	BossID = rd() % 99999 + 1;                   // Get random ID value (up to 5 digits)
	while (IDExists(BossID))                     // Ensure the ID is unique
		BossID = rd() % 99999 + 1;
	sight = rd() % 3 + 3;                        // Sight ranges from 3 to 5
	speed = rd() % 3 + 2;                        // Speed ranges from 2 to 4
	health = rd() % 11 + 40 + (50 * (lv - 1));   // Health ranges from 40 to 50 (values get higher by 50 per level as dungeon level increases)
	ATK = rd() % 4 + 10 + (5 * (lv - 1));        // ATK ranges from 10 to 13 (values get higher by 5 per level as dungeon level increases)

	if (!type)
		ai = new AI(e_queue, image, BOSS_MELEE, sight, speed, health, ATK);
	else
		ai = new AI(e_queue, image, BOSS_RANGER, sight, speed, health, ATK);

	ai->SetSpawn(*dungeon);
	ai->SetXPosition(x);
	ai->SetYPosition(y);

	group.insert(std::pair<int, AI*>(BossID, ai));
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
void AI_Group::ProcessAll(ALLEGRO_EVENT &ev, Player &p)
{
	if (ev.type != ALLEGRO_EVENT_TIMER)
		return;

	for (std::map<int, AI*>::iterator it = group.begin(); it != group.end(); it++)
	{
		// Ignore dead AI
		if ((it->second)->GetState() == DEAD)
			continue;
		(it->second)->ProcessAI(ev, p);
	}
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
//
// Check to see if the player's bound points are colliding with any of the AI in the group near
// the player.
//
bool AI_Group::CollideWithAI(int pb_x, int pb_y)
{
	for (std::map<int, AI*>::iterator it = group.begin(); it != group.end(); it++)
	{
		// Ignore dead AI
		if ((it->second)->GetState() == DEAD)
			continue;
		if ((it->second)->CollusionBlock(pb_x, pb_y))
			return true;
	}

	return false;
}
//
// Similar to the above function, but with weapon bound points and damage.
//
void AI_Group::HitAI(int w_x, int w_y, int w_d)
{
	// Ignore inactive weapons
	if (!w_d)
		return;

	for (std::map<int, AI*>::iterator it = group.begin(); it != group.end(); it++)
	{
		// Ignore dead AI
		if ((it->second)->GetState() == DEAD)
			continue;
		(it->second)->WeaponHit(w_x, w_y, w_d);
	}
}
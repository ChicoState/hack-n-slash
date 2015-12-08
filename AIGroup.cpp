//
// File: AIGroup.cpp
// Author: James Beller
// Group: Hack-'n-Slash
// Date: 12/4/2015
//
#include "AI.h"
#include "AIGroup.h"

//
// This AI_Group constructor.
//
// @Param e_q - Pointer to the event queue to be used for this group
//
AI_Group::AI_Group(ALLEGRO_EVENT_QUEUE* e_q) : e_queue(e_q), drops(e_q), BossActive(false)
{
	melee_sprite = al_load_bitmap("AI_Melee.png");
	ranger_sprite = al_load_bitmap("AI_Ranger.png");
	boss_melee_sprite = al_load_bitmap("AI_Boss_Melee.png");
	boss_ranger_sprite = al_load_bitmap("AI_Boss_Ranger.png");
}
//
// This function checks the container to see if the specified ID exists in the container.
//
// @Return: True if the id exists, false otherwise
// @Param id - The ID to check
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
// The event handler for the group.
//
// @Param ev - Allegro event variable
//
void AI_Group::EventHandler(ALLEGRO_EVENT &ev)
{
	// Handler for pickups
	drops.Event_Handler(ev);


	// Spawn the boss when SPAWN_BOSS_EVENT emits. Only one boss is allowed per group.
	if (ev.type == SPAWN_BOSS_EVENT && !BossActive)
		SpawnBoss(ev.user.data1, ev.user.data2);
	// Spawn a pickup when an AI is killed
	else if (ev.type == AI_KILLED_EVENT || ev.type == BOSS_KILLED_EVENT)
		drops.SpawnObjectRandom(AVec2i(ev.user.data1, ev.user.data2), 25);
}
//
// This function checks to see if the given AI overlaps with anyone else in the group.
//
// @Return - True if the AI overlaps with another AI in the group, false otherwise
// @Param ai - Pointer to the AI used to check the container for overlap
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
// This function checks to see if the given AI overlaps with the player's starting position.
//
// @Return - True if the ai overlaps with the player's starting position in the dungeon, false otherwise
// @Param ai - Pointer to the AI used to check for overlap
//
bool AI_Group::OverlapWithPlayerStart(AI* ai)
{
	AVec2f pos = ai->GetActiveDungeon()->GetStartPosition();

	if (ai->GetXPosition() == pos.x() && ai->GetYPosition() == pos.y())
		return true;
	else
		return false;
}
//
// This function sets a random spawn point for an AI without overlapping with the player's starting
// position and any other AI in the group
//
// @Param ai - Pointer to the AI in question
//
void AI_Group::SetRandomSpawn(AI* ai)
{
	ai->SetSpawn(*dungeon);  // Set a spawn point
	// If the AI overlaps with another AI or the player's starting position, move it away from it.
	if (Overlap(ai) || OverlapWithPlayerStart(ai))
	{
		ai->SetXPosition(ai->GetXPosition() + Random(0, T_SIZE));
		ai->SetYPosition(ai->GetYPosition() + Random(0, T_SIZE));
	}
}
//
// This function sets up the group with a given number of randomly generated AI. This function also sets the
// group's dungeon pointer, meaning this function MUST be called first before calling AddRandom or SpawnBoss.
//
// @Param n - The number of AI to spawn
// @Param d - The dungeon where the AI will be spawned
//
void AI_Group::RandomSetup(int n, DungeonGenerator &d)
{
	// If the group is not empty, clear it first
	if (!group.empty())
		GroupClear();

	BossActive = false;
	dungeon = &d;

	for (int i = 0; i < n; i++)
		AddRandom();
}
//
// This function generates a new AI with random attributes and then adds it to the group.
//
void AI_Group::AddRandom()
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
	health = rd() % 5 + 18 + ((15 + Random(1, (lv + 6))) * (lv - 1)); // Health ranges from 50 to 55 (values get higher by 20 per level as dungeon level increases)
	ATK = rd() % 4 + 5 + (2 * (lv - 1));       // ATK ranges from 5 to 8 (values get higher by 2 per level as dungeon level increases)

	if (!type)
		ai = new AI(e_queue, melee_sprite, MELEE, sight, speed, health, ATK);
	else
		ai = new AI(e_queue, ranger_sprite, RANGER, sight, speed, health, ATK);

	SetRandomSpawn(ai);
	group.insert(std::pair<int, AI*>(id, ai));
}
//
// This function generates a boss AI for the dungeon at specified coordinates.
//
// @Param x - x coordinate of the spawn point
// @Param y - y coordinate of the spawn point
//
void AI_Group::SpawnBoss(int x, int y)
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
	health = rd() % 11 + 40 + (30 * (lv - 1));   // Health ranges from 40 to 50 (values get higher by 30 per level as dungeon level increases)
	ATK = rd() % 4 + 10 + (5 * (lv - 1));        // ATK ranges from 10 to 13 (values get higher by 5 per level as dungeon level increases)

	if (!type)
		ai = new AI(e_queue, boss_melee_sprite, BOSS_MELEE, sight, speed, health, ATK);
	else
		ai = new AI(e_queue, boss_ranger_sprite, BOSS_RANGER, sight, speed, health, ATK);

	ai->SetSpawn(*dungeon);
	ai->SetXPosition(x);
	ai->SetYPosition(y);

	group.insert(std::pair<int, AI*>(BossID, ai));
}
//
// This function deallocates all AI and clear the container.
//
void AI_Group::GroupClear()
{
	for (std::map<int, AI*>::iterator it = group.begin(); it != group.end(); it++)
		delete it->second;
	group.clear();
}
//
// This function processes all AI in the group.
//
// @Param ev - Allegro event variable
// @Param p - The player each AI will deal with
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
// This function draws everyone in the group on the screen. It also draws all
// pickups.
//
void AI_Group::DrawAll()
{
	drops.Draw();
	for (std::map<int, AI*>::iterator it = group.begin(); it != group.end(); it++)
		(it->second)->Draw();
}
//
// This function checks to see if the player's bound points are colliding with any of the AI in the group near
// the player.
//
// @Return - True if there's collusion, false otherwise
// @Param pb_x - The player's x bound point
// @Param pb_y - The player's y bound point
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
// This function makes active weapons hit each AI that collide with the weapon's bound points.
//
// @Param w_x - The weapon's x bound point
// @Param w_y - The weapon's y bound point
// @Param w_d - The amount of damage the weapon deals
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

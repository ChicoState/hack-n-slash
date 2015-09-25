/**
 * File: AI.cpp
 * Author: James Beller
 * Group: Hack-'n-Slash
 * Date: 9/24/2015
 */
#include "AI.h"

/**
 * Makes the AI move 1 tile in one of the eight directions (N, NE, E, SE, S, SW, W, and NW)
 * depending on the player's current location.
 */
void AI::MoveTowardPlayer(Player p)
{
	int ai_x = ai_character->GetXPosition();
	int ai_y = ai_character->GetYPosition();
	int p_x = p.GetXPosition();
	int p_y = p.GetYPosition();
	float deltaX = (p_x - ai_x);
	if (!deltaX)
		deltaX = 0;
	else if (deltaX < 0)
		deltaX = -1;
	else
		deltaX = 1;
	float deltaY = (p_y - ai_y);
	if (!deltaY)
		deltaY = 0;
	else if (deltaY < 0)
		deltaY = -1;
	else
		deltaY = 1;
	ai_character->SetXPosition(ai_x + deltaX);
	ai_character->SetYPosition(ai_y + deltaY);
}
/**
 * Same as AI::MoveTowardPlayer, but the AI will move toward the target coordinates specified
 * by tar_x and tar_y.
 */
void AI::MoveTowardTarget()
{
	int ai_x = ai_character->GetXPosition();
	int ai_y = ai_character->GetYPosition();
	float deltaX = (tar_x - ai_x);
	if (!deltaX)
		deltaX = 0;
	else if (deltaX < 0)
		deltaX = -1;
	else
		deltaX = 1;
	float deltaY = (tar_y - ai_y);
	if (!deltaY)
		deltaY = 0;
	else if (deltaY < 0)
		deltaY = -1;
	else
		deltaY = 1;
	ai_character->SetXPosition(ai_x + deltaX);
	ai_character->SetYPosition(ai_y + deltaY);
}
/**
 * Check to see if the AI can see the player. Returns true if it can, false otherwise.
 */
bool AI::SeePlayer(Player p)
{
	int ai_x = ai_character->GetXPosition();
	int ai_y = ai_character->GetYPosition();
	int p_x = p.GetXPosition();
	int p_y = p.GetYPosition();
	float d = sqrt(pow((float)p_x - ai_x, 2) + pow((float)p_y - ai_y, 2));
	if (sight > d)
		return true;
	else
		return false;
}
/**
 * The AI in action.
 */
void AI::ProcessAI(Player player)
{
	if (state == IDLE)
	{
		if (SeePlayer(player))
			state = CHASE;
	}
	else if (state == CHASE)
	{
		if (!SeePlayer(player))
			state = IDLE;
		else
			MoveTowardPlayer(player);
	}
}
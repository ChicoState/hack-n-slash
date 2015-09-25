/**
 * File: AI.h
 * Author: James Beller
 * Group: Hack-'n-Slash
 * Date: 9/24/2015
 */
#ifndef __AI_H__
#define __AI_H__

#include <math.h>
#include "Player.h"
#include "Character.h"

// NOTE: More features will be added later.
enum AI_STATE{ IDLE, CHASE };
enum AI_TYPE{ MELEE };

class AI
{
private:
	int state;                       // The AI's current behavior (IDLE, ATTACK, etc.)
	int sight;                       // How far the AI can see
	int type;                        // The type of AI (MELEE, RANGER, etc)
	int tar_x;                       // Target x coordinate
	int tar_y;                       // Target y coordinate
	Character *ai_character;         // The character the AI will control
public:
	AI(int t, int s, Character *p) : state(IDLE), type(t), sight(s), ai_character(p) {}
	~AI() { delete ai_character; }
	int GetState() { return state; }
	void SetState(int s) { state = s; }
	int GetType() { return type; }
	int GetTargetX() { return tar_x; }
	int GetTargetY() { return tar_y; }
	void SetTargetX(int x) { tar_x = x; }
	void SetTargetY(int y) { tar_y = y; }
	int GetXPosition() { return ai_character->GetXPosition(); }
	int GetYPosition() { return ai_character->GetYPosition(); }
	int SetXPosition(int x) { ai_character->SetXPosition(x); }
	int SetYPosition(int y) { ai_character->SetXPosition(y); }
	void MoveTowardPlayer(Player p); // Move towards the player
	void MoveTowardTarget();         // Move towards target coordinates (towards tar_x and tar_y)
	bool SeePlayer(Player p);        // Check if the AI can see the player
	void ProcessAI(Player p);        // Process the AI based off of the player's current status (i.e. current position)
};

#endif
//Scott Backer

//ScoreTimer - Keeps track of and iterates the score for the player

#include "PlayerScoreCalculator.h"

//Constructor for the score calculator
PlayerScoreCalculator::PlayerScoreCalculator()
{
	//initialize member variables
	m_EnemyScoreIncrement = 10;
	m_BossScoreIncrement = 100;

	m_CurrentPlayerScore = 0;
}

//!Handles all the functions for the score calculator that need to be called every update
//In - 
//		ALLEGRO_EVENT& InputAlEvent - The global event handler for the game
void PlayerScoreCalculator::EventHandler(ALLEGRO_EVENT& InputAlEvent)
{
	//Check custom events

	if(InputAlEvent.type == AI_KILLED_EVENT)
	{
		m_CurrentPlayerScore += 10;
	}

	if(InputAlEvent.type == BOSS_KILLED_EVENT)
	{
		m_CurrentPlayerScore += 100;
	}
}

//Adds score to the player for a defeated enemy
void PlayerScoreCalculator::AddEnemyKilledScore()
{
	m_CurrentPlayerScore += m_EnemyScoreIncrement;
}

//Adds score to the player for a defeated boss
void PlayerScoreCalculator::AddBossKilledScore()
{
	m_CurrentPlayerScore += m_BossScoreIncrement;
}

//Calculates the player score according to the timed calculation
//In - 
//		int NewXPosition - the new x position for the player
//Out - 
//		int - the calculated timed score for the player
int PlayerScoreCalculator::CalculateTimedScore(const ALLEGRO_TIMER* InputTimer)
{
	//get the time in seconds
	int TimerTime = (al_get_timer_count(InputTimer) / 60);

	//calculate the final score
	int CalculatedScore = round(100000 / (TimerTime * 0.01)) + m_CurrentPlayerScore;

	return CalculatedScore;
}

//!Gets and returns the current score of the player
//Out
//		int - the current player score
int PlayerScoreCalculator::GetCurrentPlayerScore()
{
	return m_CurrentPlayerScore;
}
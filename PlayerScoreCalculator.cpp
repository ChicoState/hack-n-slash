//Scott Backer

//ScoreTimer - Keeps track of and iterates the score for the player

#include "PlayerScoreCalculator.h"

//Constructor for the score calculator
PlayerScoreCalculator::PlayerScoreCalculator()
{
	//initialize member variables
	m_EnemyScoreIncrement = 50;
	m_BossScoreIncrement = 500;
	m_LevelUpScoreIncrement = 250;

	m_CurrentPlayerScore = 0;
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

//Adds score to the player for a defeated boss
void PlayerScoreCalculator::AddLevelUpScore()
{
	m_CurrentPlayerScore += m_LevelUpScoreIncrement;
}

//Calculates the player score according to the timed calculation
//In - 
//		int NewXPosition - the new x position for the player
//		int CurrentGameScale - the current game scale number
//Out - 
//		int - the calculated timed score for the player
int PlayerScoreCalculator::CalculateTimedScore(const ALLEGRO_TIMER* InputTimer, int CurrentGameScale)
{
	//get the time in seconds
	int TimerTime = (al_get_timer_count(InputTimer) / 60);

	//calculate the final score
	int CalculatedScore = m_CurrentPlayerScore - (TimerTime / CurrentGameScale);

	if(CalculatedScore < 0)
	{
		CalculatedScore = 0;
	}

	//return calculated score
	return CalculatedScore;
}

//Resets the player score
void PlayerScoreCalculator::ResetScore()
{
	//reset the player score
	m_CurrentPlayerScore = 0;
}

//!Gets and returns the current score of the player
//Out
//		int - the current player score
int PlayerScoreCalculator::GetCurrentPlayerScore()
{
	return m_CurrentPlayerScore;
}
//Scott Backer

//ScoreTimer - Keeps track of and iterates the score for the player

#ifndef PLAYERSCORECALCULATOR_H
#define PLAYERSCORECALCULATOR_H

#include <allegro5\allegro.h>
#include <math.h>

#include "Utility.h"

class PlayerScoreCalculator
{
public:
	PlayerScoreCalculator(); //Constructor for the score calculator
	void AddEnemyKilledScore(); //Adds score to the player for a defeated enemy
	void AddBossKilledScore();//Adds score to the player for a defeated boss
	void AddLevelUpScore(); //Adds score to the player for a level up
	int CalculateTimedScore(const ALLEGRO_TIMER* InputTimer, int CurrentGameScale); //Calculates the player score according to the timed calculation
	void ResetScore(); //Resets the player score

	int GetCurrentPlayerScore(); //!Gets and returns the current score of the player

private:
	int m_EnemyScoreIncrement; //the amount to incrmement the score for a killed enemy
	int m_BossScoreIncrement; //the amount to incrmement score for a killed boss
	int m_LevelUpScoreIncrement; //the amount to increment score for a level up
	int m_CurrentPlayerScore; //the current score of the player
};

#endif
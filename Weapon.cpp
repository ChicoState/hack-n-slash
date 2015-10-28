//Scott Backer

//Weapon Class CPP: Creates the use of a weapon for a player/AI and a parent class for all weapon types.

#include "Weapon.h"

//!Constructor for the weapon class
//In - 
//		ALLEGRO_EVENT& InputAlEvent - the allegro event of the game system for the weapon to use
//		int InputXBound - the x bound for the weapon
//		int InputYBound - the y bound for the weapon
//		bool IsRangedWeapon - true if the weapon is a ranged weapon else false
//		float InputAttackTime - the time in seconds the weapon will be active when used to attack
//		float InputDamage - the damage the weapon does
//		ALLEGRO_BITMAP *SpriteImage - the sprite image of the player tile
//		int InputPlayerXPosition - the starting x position of the player
//		int InputPlayerXPosition - the starting y position of the player
//		int FrameWidth - the width of one sprite frame
//		int FrameHeight - the height of one sprite frame
//		bool Collidable - true if the tile is collidable else false
//		bool Animated - true if the tile is animated else false
//		bool Continuous - true if the tile is continues else false
//		bool Looped - true if the tile should loop else false
//		int AnimFPS - the fps for the animated tile
Weapon::Weapon(ALLEGRO_EVENT& InputAlEvent, int InputXBound, int InputYBound, bool IsRangedWeapon, float InputAttackTime, float InputDamage)
{
	//initialize member variables
	m_AlEvent = InputAlEvent;
	m_XBound = InputXBound;
	m_YBound = InputYBound;
	m_IsRangedWeapon = IsRangedWeapon;
	m_IsActive = false;
	m_AttackTime = (InputAttackTime * 60);
	m_CurrentAttackCount = 0;
	m_Damage = InputDamage;
	m_DamageModifier = 1;
}

//!Handles allegro events for the weapon class
void Weapon::EventHandler()
{
	if(m_AlEvent.type = ALLEGRO_EVENT_TIMER)
	{
		//if the weapon is active watch the active timer
		if(m_IsActive)
		{
			//iterate the timer
			m_CurrentAttackCount++;

			//if the active timer is reached
			if(m_CurrentAttackCount >= m_AttackTime)
			{
				//make weapon unactive and reset timer
				m_IsActive = false;
				m_CurrentAttackCount = 0;
			}
		}
	}
}

//!Handles drawing for the weapon class
//In - 
//		int DrawXCoordinate - DrawXCoordinate - the x coordinate to draw the weapon at
//		int DrawYCoordinate - DrawXCoordinate - the y coordinate to draw the weapon at
//		int XDirection - the x direction relative to the player/AI using the weapon to draw at (use graph coordinates to calculate (Ex. 0, 1 is North or Up))
//		int YDirection - the y direction relative to the player/AI using the weapon to draw at (use graph coordinates to calculate (Ex. 0, 1 is North or Up))
void Weapon::Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection)
{
	/*
	//if the weapon is active draw this
	if(m_IsActive)
	{
		al_draw_line(DrawXCoordinate, DrawYCoordinate, DrawXCoordinate + (100 * XDirection), DrawYCoordinate + (100 * YDirection), al_map_rgb(255, 0, 0), 10);
	}

	//else if the weapon is not active draw this
	else
	{
		al_draw_line(DrawXCoordinate, DrawYCoordinate, DrawXCoordinate + (100 * XDirection), DrawYCoordinate + (100 * YDirection), al_map_rgb(0, 0, 255), 10);
	}
	*/
}

//!Resets the weapon to a non active state
void Weapon::ResetWeapon()
{
	m_IsActive = false;
	m_CurrentAttackCount = 0;
}

//!Returns whether or not the weapon is active
//Out- 
//		bool - true if the weapon is active else false
bool Weapon::IsActive()
{
	return m_IsActive;
}

//!Attacks with the weapon to make it active
void Weapon::Attack()
{
	m_IsActive = true;
	m_CurrentAttackCount = 0;
}

//!Gets and returns the weapon x bound
//Out - 
//		int - x bound of the weapon
int Weapon::GetXBound()
{
	return m_XBound;
}

//!Gets and returns the weapon y bound
//Out - 
//		int - y bound of the weapon
int Weapon::GetYBound()
{
	return m_YBound;
}

//!Gets and returns the damage the weapon deals
//Out - 
//		float - the damage the weapon deals
float Weapon::GetDamage()
{
	return (m_Damage * m_DamageModifier);
}

//Sets the damage modifier for the weapon
//In - 
//		float InputDamageModifier - the input damage modifier
void Weapon::SetDamageModifier(float InputDamageModifier)
{
	m_DamageModifier = InputDamageModifier;
}
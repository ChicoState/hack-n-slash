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
Weapon::Weapon(ALLEGRO_EVENT& InputAlEvent, /*InputWeaponSprite, */int InputXBound, int InputYBound, bool IsRangedWeapon, float InputAttackTime, float InputDamage)
{
	//initialize member variables
	m_AlEvent = InputAlEvent;
	//m_Sprite = InputWeaponSprite;
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
//In - 
//		int DrawXCoordinate - the x coordinate to draw the weapon at
//		int DrawYCoordinate - the y coordinate to draw the weapon at
//		int XDirection - the x direction relative to the player/AI using the weapon to draw at (use graph coordinates to calculate (Ex. 0, 1 is North or Up))
//		int YDirection - the y direction relative to the player/AI using the weapon to draw at (use graph coordinates to calculate (Ex. 0, 1 is North or Up))
void Weapon::EventHandler(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection)
{
	if(m_AlEvent.type = ALLEGRO_EVENT_TIMER)
	{
		//draw the weapon sprite
		Draw(DrawXCoordinate, DrawYCoordinate, XDirection, YDirection);

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


void Weapon::Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection)
{
	
}

void Weapon::Attack()
{
	m_IsActive = true;
	m_CurrentAttackCount = 0;
}

int Weapon::GetXBound()
{
	return m_XBound;
}

int Weapon::GetYBound()
{
	return m_YBound;
}

bool Weapon::IsActive()
{
	return m_IsActive;
}

float Weapon::GetDamage()
{
	return (m_Damage * m_DamageModifier);
}

void Weapon::SetDamageModifier(float InputDamageModifier)
{
	m_DamageModifier = InputDamageModifier;
}
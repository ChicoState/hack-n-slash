//Scott Backer

//Sword Weapon Class CPP: Creates a sword weapon for use with a player/AI

#include "SwordWeapon.h"

//!The constructor for the sword weapon
//In - 
//		ALLEGROEVENT& InputAlEvent - the allegro event of the game
//		ALLEGRO_BITMAP *SpriteImage - the sprite image of the sword weapon
SwordWeapon::SwordWeapon(ALLEGRO_EVENT& InputAlEvent, ALLEGRO_BITMAP *SpriteImage) : 
				Weapon(InputAlEvent, 16, 16, false, 2, 10),
				m_SwordWeaponTile(SpriteImage, 0, 0, 70, 70, true, true, false, true, 6)
{

}

//!Handles allegro events for the sword weapon class
void SwordWeapon::EventHandler()
{
	if(m_AlEvent.type = ALLEGRO_EVENT_TIMER)
	{
		//if the weapon is active watch the active timer
		if(m_IsActive)
		{
			//Update the weapon sprite tile
			m_SwordWeaponTile.Event_Handler();

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
void SwordWeapon::Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection)
{
	m_SwordWeaponTile.Draw(DrawXCoordinate, DrawYCoordinate, XDirection, YDirection, m_IsActive);
}


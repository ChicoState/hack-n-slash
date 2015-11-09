//Scott Backer

//Bow Weapon Class CPP: Creates a Bow weapon for use with a player/AI

#include "BowWeapon.h"

//!The constructor for the Bow weapon
//In - 
//		ALLEGROEVENT& InputAlEvent - the allegro event of the game
//		ALLEGRO_BITMAP *SpriteImage - the sprite image of the Bow weapon
BowWeapon::BowWeapon(ALLEGRO_EVENT& InputAlEvent, ALLEGRO_BITMAP *SpriteImage) :
				Weapon(InputAlEvent, 16, 16, true, 4, 6),
				m_BowWeaponTile(SpriteImage, 0, 0, 80, 76, true, true, false, true, 6)
{
	m_ProjectileXBound = 16;
	m_ProjectileYBound = 16;
	m_ProjectileSpeed = 12;
}

//!Handles allegro events for the Bow weapon class
void BowWeapon::EventHandler()
{
	if(m_AlEvent.type = ALLEGRO_EVENT_TIMER)
	{
		//if the weapon is active watch the active timer
		if(m_IsActive)
		{
			//iterate the timer
			m_CurrentAttackCount++;

			//Update the weapon sprite tile
			m_BowWeaponTile.Event_Handler();

			//move the projectile if a ranged weapon
			if(m_IsRangedWeapon)
			{
				if(m_Projectile != NULL)
				{
					m_Projectile->UpdatePosition();
				}
			}

			//if the active timer is reached
			if(m_CurrentAttackCount >= m_AttackTime)
			{
				//make weapon unactive and reset timer
				m_IsActive = false;
				m_CurrentAttackCount = 0;

				if(m_IsRangedWeapon)
				{
					if(m_Projectile != NULL)
					{
						m_Projectile->ResetProjectile();
					}
				}
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
void BowWeapon::Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection)
{
	m_LastDrawnXPosition = DrawXCoordinate;
	m_LastDrawnYPosition = DrawYCoordinate;

	if(XDirection == 0 && YDirection == -1)
	{
		m_LastDrawnDirection = Direction(North);
	}

	else if(XDirection == 0 && YDirection == 1)
	{
		m_LastDrawnDirection = Direction(South);
	}

	else if(XDirection == 1 && YDirection == 0)
	{
		m_LastDrawnDirection = Direction(East);
	}

	else if(XDirection == -1 && YDirection == 0)
	{
		m_LastDrawnDirection = Direction(West);
	}

	else
	{
		m_LastDrawnDirection = Direction(North);
	}

	m_BowWeaponTile.Draw(DrawXCoordinate, DrawYCoordinate, XDirection, YDirection, m_IsActive);

	//if active draw the projectile
	if(m_IsActive)
	{
		m_Projectile->Draw();
	}
}


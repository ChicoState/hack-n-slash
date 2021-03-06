//Scott Backer

//Bow Weapon Class CPP: Creates a Bow weapon for use with a player/AI

#include "BowWeapon.h"

//!The constructor for the Bow weapon
//In - 
//		ALLEGRO_EVENT_QUEUE* InputEventQueue = the allegro event queue of the game
//		ALLEGROEVENT& InputAlEvent - the allegro event of the game
BowWeapon::BowWeapon(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent) :
				Weapon(InputEventQueue, InputAlEvent, 16, 16, true, 0.25f, 6),
				m_BowWeaponTile(0, 0, 80, 76, true, true, false, true, 6)
{
	//set member variables
	m_ProjectileXBound = 16;
	m_ProjectileYBound = 16;
	m_ProjectileSpeed = 12;
	m_Projectile = new Projectile(m_ProjectileXBound, m_ProjectileYBound, m_ProjectileSpeed, 0, 0, 0, -1);
	m_Projectile->ResetProjectile();
	
	//initilaze member variables
	m_OnActive = true;
	
	al_init_user_event_source(&m_ProjectileEventSource);
	al_register_event_source(m_EventQueue, &m_ProjectileEventSource);
}

//!Handles allegro events for the Bow weapon class
void BowWeapon::EventHandler()
{
	//if the weapon is active watch the active timer
	if(m_IsActive)
	{
		//if it is the weapons first active frame
		if(m_OnActive)
		{
			//emit the event source that the projectile has moved
			m_AlEvent.user.type = CUSTOM_EVENT_ID(PROJECTILE_EVENT);
			m_AlEvent.user.data1 = (intptr_t)m_Projectile;
			al_emit_user_event(&m_ProjectileEventSource, &m_AlEvent, NULL);
			m_OnActive = false;
		}

		//if the active timer is reached
		if(m_CurrentAttackCount >= m_AttackTime || m_Projectile->IsAtRestingPosition())
		{
			//make weapon unactive and reset timer
			m_IsActive = false;
			m_CurrentAttackCount = 0;
			//m_OnActive = true;

			//reset projectile
			if(m_IsRangedWeapon)
			{
				if(m_Projectile != NULL)
				{
					m_Projectile->ResetProjectile();
				}
			}
		}

		else
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
	//keep track of last coordinates
	m_LastDrawnXPosition = DrawXCoordinate;
	m_LastDrawnYPosition = DrawYCoordinate;

	//set draw direction

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

	//draw sprite
	m_BowWeaponTile.Draw(DrawXCoordinate, DrawYCoordinate, XDirection, YDirection, m_IsActive);

	//if active draw the projectile
	if(m_IsActive)
	{
		m_Projectile->Draw();
	}
}


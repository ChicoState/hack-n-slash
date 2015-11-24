//Scott Backer

//Sword Weapon Class CPP: Creates a sword weapon for use with a player/AI

#include "SwordWeapon.h"

<<<<<<< HEAD
SwordWeapon::SwordWeapon(ALLEGRO_EVENT& InputAlEvent) : Weapon(InputAlEvent, 16, 16, false, 2, 10)
=======
//!The constructor for the sword weapon
//In - 
//		ALLEGRO_EVENT_QUEUE* InputEventQueue = the allegro event queue of the game
//		ALLEGROEVENT& InputAlEvent - the allegro event of the game
SwordWeapon::SwordWeapon(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent) : 
				Weapon(InputEventQueue, InputAlEvent, 16, 16, false, 2, 10),
				m_SwordWeaponTile(0, 0, 70, 70, true, true, false, true, 6)
>>>>>>> Scott
{

}

<<<<<<< HEAD
void SwordWeapon::Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection)
{
	if(m_IsActive)
	{
		al_draw_line(DrawXCoordinate, DrawYCoordinate, DrawXCoordinate + (30 * XDirection), DrawYCoordinate + (30 * YDirection), al_map_rgb(255, 0, 0), 10);
=======
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
>>>>>>> Scott
	}

	else
	{
<<<<<<< HEAD
		al_draw_line(DrawXCoordinate, DrawYCoordinate, DrawXCoordinate + (30 * XDirection), DrawYCoordinate + (30 * YDirection), al_map_rgb(0, 0, 255), 10);
	}
=======
		m_LastDrawnDirection = Direction(North);
	}

	//draw sprite
	m_SwordWeaponTile.Draw(DrawXCoordinate, DrawYCoordinate, XDirection, YDirection, m_IsActive);
>>>>>>> Scott
}


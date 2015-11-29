//Scott Backer

//Sword Weapon Class CPP: Creates a sword weapon for use with a player/AI

#include "SwordWeapon.h"

//!The constructor for the sword weapon
//In - 
<<<<<<< HEAD
//		ALLEGRO_EVENT_QUEUE* InputEventQueue = the allegro event queue of the game
//		ALLEGROEVENT& InputAlEvent - the allegro event of the game
SwordWeapon::SwordWeapon(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent) : 
				Weapon(InputEventQueue, InputAlEvent, 16, 16, false, 0.5, 10),
				m_SwordWeaponTile(0, 0, 70, 70, true, true, false, true, 6)
=======
//		ALLEGROEVENT& InputAlEvent - the allegro event of the game
//		ALLEGRO_BITMAP *SpriteImage - the sprite image of the sword weapon
SwordWeapon::SwordWeapon(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent, ALLEGRO_BITMAP *SpriteImage) : 
				Weapon(InputEventQueue, InputAlEvent, 16, 16, false, 2, 10),
				m_SwordWeaponTile(SpriteImage, 0, 0, 70, 70, true, true, false, true, 6)
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
{
	m_OnActive = true;

	al_init_user_event_source(&m_SwordActiveEventSource);
	al_register_event_source(m_EventQueue, &m_SwordActiveEventSource);
}

<<<<<<< HEAD
//Destructor for the sword weapon class
SwordWeapon::~SwordWeapon()
{
	//al_unregister_event_source(m_EventQueue, &m_SwordActiveEventSource);
	//al_destroy_user_event_source(&m_SwordActiveEventSource);
}

=======
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
//!Handles allegro events for the sword weapon class
void SwordWeapon::EventHandler()
{
	if(m_AlEvent.type = ALLEGRO_EVENT_TIMER)
	{
		//if the weapon is active watch the active timer
		if(m_IsActive)
		{
<<<<<<< HEAD
			//emit the event source that the projectile has moved
			m_AlEvent.user.type = CUSTOM_EVENT_ID(MELEEATTACK_EVENT);
			al_emit_user_event(&m_SwordActiveEventSource, &m_AlEvent, NULL);
			m_OnActive = false;

=======
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
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
<<<<<<< HEAD
				m_OnActive = true;
=======
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
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
<<<<<<< HEAD
	//keep track of last coordinates
	m_LastDrawnXPosition = DrawXCoordinate;
	m_LastDrawnYPosition = DrawYCoordinate;

	//set draw direction

=======
	m_LastDrawnXPosition = DrawXCoordinate;
	m_LastDrawnYPosition = DrawYCoordinate;

>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
	if(XDirection == 0 && YDirection == -1)
	{
		m_LastDrawnDirection = Direction(North);
	}

	else if(XDirection == 0 && YDirection == 1)
<<<<<<< HEAD
	{
		m_LastDrawnDirection = Direction(South);
	}

	else if(XDirection == 1 && YDirection == 0)
	{
=======
	{
		m_LastDrawnDirection = Direction(South);
	}

	else if(XDirection == 1 && YDirection == 0)
	{
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
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

<<<<<<< HEAD
	//draw sprite
=======
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
	m_SwordWeaponTile.Draw(DrawXCoordinate, DrawYCoordinate, XDirection, YDirection, m_IsActive);
}


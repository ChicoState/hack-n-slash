//Created by:		Ryan Nolan-Hieb
//Date updated:		9/16/15
//Last update by:	N/A
//Reason for update:N/A

#ifndef __UTILITY__
#define __UTILITY__

#include <map>
#include <string>

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>

enum CUSTOM_EVENT_ID 
{ 
	PROJECTILE_EVENT = 1025, 
	PLAYERPOSITION_EVENT, 
	FOODPICKUP_EVENT, 
	SPEEDPICKUP_EVENT, 
	STRENGTHPICKUP_EVENT,
	AI_KILLED_EVENT
};


//Global utility class for keeping track of global key presses/game state/if game is over/etc..
//Do NOT create another variable of this class. Include this class and use "MainUtility" variable
//instead (located at bottom of class declaration)
class Utility
{
private:
	bool m_GameOver;


public:
	std::map<std::string, bool> m_Keys;

	Utility()
	{
		m_GameOver = false;

		m_Keys["W"] = 0;
		m_Keys["A"] = 0;
		m_Keys["S"] = 0;
		m_Keys["D"] = 0;
	}

	void Set_GameOver(bool);
	bool Get_GameOver() const;
	void InitAllegro();
};

extern Utility MainUtility;

#endif
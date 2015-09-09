#ifndef __CHARACTER__
#define __CHARACTER__

#include <string>

#include <allegro5\allegro.h>

class Character
{
private:
	//Basic Variables
	std::string m_Name;
	int m_HP;
	int m_MaxHP;
	short int m_Level;

	//Location
	int m_PosX;
	int m_PosY;

	//Allegro Variables
	ALLEGRO_EVENT_QUEUE *m_EventQueue;

public:
	virtual int Event_Handler();

};

#endif
//Created by:		Ryan Nolan-Hieb
//Date updated:		9/3/15
//Last update by:	N/A
//Reason for update:N/A
#ifndef __DISPLAY__
#define __DISPLAY__

#include <allegro5\allegro.h>

#include "Utility.h"

class Display
{

private:
	
	const int m_ScreenWidth = 1280;
	const int m_ScreenHeight = 720;

	ALLEGRO_DISPLAY		*m_Display;
	ALLEGRO_EVENT_QUEUE *m_EventQueue;

public:
	
	Display(ALLEGRO_EVENT_QUEUE* EventQueue) :
		m_EventQueue(EventQueue)
	{
		m_Display = al_create_display(m_ScreenWidth, m_ScreenHeight);
		al_register_event_source(m_EventQueue, al_get_display_event_source(m_Display));
	}

	~Display()
	{
		al_unregister_event_source(m_EventQueue, al_get_display_event_source(m_Display));
		al_destroy_display(m_Display);
	}

	int Event_Handler();

	bool TestDisplay();
};

#endif
//Created by:		Ryan Nolan-Hieb
//Date updated:		9/3/15
//Last update by:	N/A
//Reason for update:N/A

#include <iostream>

#include "Display.h"
#include "Utility.h"

int Display::Event_Handler()
{
	ALLEGRO_EVENT ev;
	al_wait_for_event(m_EventQueue, &ev);

	if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //Main program window closed...exit game.
	{
		MainUtility.Set_GameOver(1);
	}
	
	if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
	{
		return 0;
	}

	return 0;
}

bool Display::TestDisplay()
{
	if (!m_Display)
		return 0;
	
	return 1;
}


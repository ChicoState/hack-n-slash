//Created by:		Ryan Nolan-Hieb
//Date updated:		9/9/15
//Last update by:	N/A
//Reason for update:N/A

#include <iostream>

#include "Display.h"
#include "Utility.h"

int Display::Event_Handler(ALLEGRO_EVENT &ev)
{
	if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //Main program window closed...exit game.
	{
		MainUtility.Set_GameOver(1);
	}
	else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
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

ALLEGRO_DISPLAY *Display::Get_Display()
{
	return m_Display;
}

void Display::Draw()
{
	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));
}
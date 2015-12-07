//Created by:		Ryan Nolan-Hieb

#include <iostream>

#include "Display.h"
#include "Utility.h"

int Display::Event_Handler(ALLEGRO_EVENT &ev)
{
	if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //Main program window closed...exit game.
	{
		return 1;
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
	al_clear_to_color(al_map_rgb(22, 18, 17));
}

void Display::SetDisplayAsTarget()
{
	al_set_target_bitmap(al_get_backbuffer(m_Display));
}
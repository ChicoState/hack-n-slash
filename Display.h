//Created by:		Ryan Nolan-Hieb
#ifndef __DISPLAY__
#define __DISPLAY__

#include <allegro5\allegro.h>

#include "Utility.h"

//Wrapper class for Allegro Display
class Display
{

private:
	
	const int m_ScreenWidth = 1920; //Screen Dimension width
	const int m_ScreenHeight = 1080;//Screen Dimension height

	ALLEGRO_DISPLAY		*m_Display; //Allegro Display
	ALLEGRO_EVENT_QUEUE *m_EventQueue; //pointer to main event queue
	
public:
	
	Display(ALLEGRO_EVENT_QUEUE* EventQueue) :
		m_EventQueue(EventQueue)
	{
		m_Display = al_create_display(m_ScreenWidth, m_ScreenHeight); //create the display
		al_register_event_source(m_EventQueue, al_get_display_event_source(m_Display)); //register it as an event source
	}

	~Display()
	{
		al_unregister_event_source(m_EventQueue, al_get_display_event_source(m_Display));
		al_destroy_display(m_Display);
	}

	//Basic setters and getters 

	int Get_ScreenWidth(){ return m_ScreenWidth; }
	int Get_ScreenHeight(){ return m_ScreenHeight; }

	//Core functions
	
	int Event_Handler(ALLEGRO_EVENT&); //hnadles the main events
	ALLEGRO_DISPLAY *Get_Display(); //get a pointer to the display
	bool TestDisplay(); //Makes sure the display started correctly
	void SetDisplayAsTarget(); //Makes the display the render target again
	void Draw(); //Draws the display
};

#endif
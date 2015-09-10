//Created by:		Ryan Nolan-Hieb
//Date updated:		9/9/15
//Last update by:	N/A
//Reason for update:N/A

#include <iostream>

#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>

#include "Utility.h"
#include "Display.h"

//Globals-----------------------------------------------
Utility MainUtility;

int main(void)
{
	//Initialize allegro--------------------------------
	MainUtility.InitAllegro();
	if (MainUtility.Get_GameOver())
	{
		return -1;
	}

	//variables-----------------------------------------
	const int FPS = 60;

	//Allegro variables---------------------------------
	ALLEGRO_EVENT_QUEUE *Event_Queue = NULL;
	ALLEGRO_TIMER *Timer = NULL;

	Event_Queue = al_create_event_queue();				
	Timer = al_create_timer(1.0 / FPS);

	Display MainDisplay(Event_Queue);
	
	if (!MainDisplay.TestDisplay())
	{
		return -1;
	}

	al_register_event_source(Event_Queue, al_get_timer_event_source(Timer));

	al_start_timer(Timer);
	//Main game loop------------------------------------
	while (!MainUtility.Get_GameOver())
	{
		MainDisplay.Event_Handler();

		MainDisplay.Draw();
	}


	//Game Ending--------------------------------------

	return 0;
}
//Created by:		Ryan Nolan-Hieb
//Date updated:		9/3/15
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
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
		
	event_queue = al_create_event_queue();				
	timer = al_create_timer(1.0 / FPS);

	Display MainDisplay(event_queue);
	
	if (!MainDisplay.TestDisplay())
	{
		return -1;
	}

	//Main game loop------------------------------------
	while (!MainUtility.Get_GameOver())
	{
		MainDisplay.Event_Handler();
	}


	//Game Ending--------------------------------------

	return 0;
}
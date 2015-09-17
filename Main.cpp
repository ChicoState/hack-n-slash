//Created by:		Ryan Nolan-Hieb
//Date updated:		9/9/15
//Last update by:	N/A
//Reason for update:N/A

#include <iostream>
#include <time.h>

#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>

#include "Utility.h"
#include "Display.h"
#include "DungeonGenerator.h"



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

	srand(time(NULL));

	//variables-----------------------------------------
	const int FPS = 60;
	DungeonGenerator Dungeon;
	//Dungeon.GenerateDungeon();

	//Allegro variables---------------------------------
	ALLEGRO_EVENT_QUEUE *Event_Queue = NULL;
	ALLEGRO_TIMER *Timer = NULL;

	//TERRAIN RELATED CODE-------------------------------
	ALLEGRO_BITMAP *image = al_load_bitmap("TestRender.jpg");

	//----------------------------------------------------

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
		ALLEGRO_EVENT ev;
		al_wait_for_event(Event_Queue, &ev);
		
		MainDisplay.Event_Handler(ev); 
		
		
		//Code Dealing with drawing to the screen goes within this if statement
		if (al_is_event_queue_empty(Event_Queue))
		{
			al_draw_bitmap(image, 0, 0, 0);
			MainDisplay.Draw();
		}
	}

	
	//Game Ending--------------------------------------

	return 0;
}
//Created by:		Ryan Nolan-Hieb
//Date updated:		10/9/15
//Last update by:	N/A
//Reason for update:N/A

#include <iostream>
#include <time.h>

#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>

#include "AI.h"
#include "Player.h"
#include "Camera.h"
#include "Utility.h"
#include "Display.h"
#include "TerrainTile.h"
#include "DungeonGenerator.h"
#include "TerrainGenerator.h"

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

	ALLEGRO_BITMAP *PlayerImage = al_load_bitmap("Player_Sprite.png");
	Player MainPlayer(PlayerImage, MainDisplay.Get_ScreenWidth(), MainDisplay.Get_ScreenHeight(), Event_Queue);
	Camera MainCamera(Event_Queue);
	AI MainAIController(MELEE, 5);


	TerrainGenerator Terrain(1);
	Terrain.generateTerrain();

	DungeonGenerator Dungeon(Event_Queue);
	Dungeon.GenerateDungeon(MainDisplay);

	MainPlayer.SetXPosition(Dungeon.GetStartPosition().x());
	MainPlayer.SetYPosition(Dungeon.GetStartPosition().y());


	al_register_event_source(Event_Queue, al_get_timer_event_source(Timer));
	al_register_event_source(Event_Queue, al_get_keyboard_event_source());
	al_register_event_source(Event_Queue, al_get_mouse_event_source());
	

	al_start_timer(Timer);
	//Main game loop------------------------------------
	while (!MainUtility.Get_GameOver())
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(Event_Queue, &ev);
		
		MainPlayer.EventHandler(ev, MainCamera.GetMouseXWorldCoordinate(), MainCamera.GetMouseYWorldCoordinate());
		//MainAIController.ProcessAI(MainPlayer, Dungeon);
		MainCamera.EventHandler(ev, MainPlayer.GetXPosition(), MainPlayer.GetYPosition());
		MainDisplay.Event_Handler(ev); 
		
		if (Dungeon.Get_Map()->CheckMapCollision(Vec2f(MainPlayer.GetCurrentMovingXPosition(), MainPlayer.GetCurrentMovingYPosition())))
			MainPlayer.MovementColliding();

		//Code Dealing with drawing to the screen goes within this if statement
		if (al_is_event_queue_empty(Event_Queue))
		{
			Dungeon.Draw();
			MainPlayer.DrawPlayer();
			//Terrain.draw();

			//Draw display last
			MainDisplay.Draw();
		}
	}

	
	//Game Ending--------------------------------------

	return 0;
}
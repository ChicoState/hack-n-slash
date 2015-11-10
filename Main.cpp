//Created by:		Ryan Nolan-Hieb
//Date updated:		10/9/15
//Last update by:	N/A
//Reason for update:N/A

#include <iostream>
#include <time.h>

#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>

#include "AI.h"
#include "AIGroup.h"
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

	ALLEGRO_BITMAP *DungeonTiles = al_load_bitmap("Test.png");
	ALLEGRO_BITMAP *AIImage = al_load_bitmap("AI_Sprite.png");
	ALLEGRO_BITMAP *PlayerImage = al_load_bitmap("Player_Sprite.png");
	ALLEGRO_BITMAP *BowImage = al_load_bitmap("Bow_Sprite.png");
	ALLEGRO_BITMAP *SwordImage = al_load_bitmap("Sword_Sprite.png");

	Player MainPlayer(PlayerImage, SwordImage, BowImage, MainDisplay.Get_ScreenWidth(), MainDisplay.Get_ScreenHeight(), Event_Queue);
	Camera MainCamera(Event_Queue);
	AI_Group TestAIGroup;  // Instance of an AI_Group

	TerrainGenerator Terrain(1);
	Terrain.generateTerrain();

	DungeonGenerator Dungeon(Event_Queue, &MainPlayer);
	Dungeon.GenerateDungeon(MainDisplay);

	MainPlayer.SetXPosition(Dungeon.GetStartPosition().x());
	MainPlayer.SetYPosition(Dungeon.GetStartPosition().y());

	TestAIGroup.RandomSetup(4, Dungeon, AIImage);  // Generates 4 AI with random attributes in the group. Their spawn points will also be set randomly.
	TestAIGroup.GetPathToPlayer(MainPlayer);  // For testing, generate a path to the player's start position from each AI


	ALLEGRO_EVENT_SOURCE ProjectileEvent;
	al_init_user_event_source(&ProjectileEvent);

	

	
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
		TestAIGroup.ProcessAll(ev, MainPlayer);  // Process each AI in the group
		MainCamera.EventHandler(ev, MainPlayer.GetXPosition(), MainPlayer.GetYPosition());
		MainDisplay.Event_Handler(ev); 
		Dungeon.Event_Handler(ev);
		
		// Collide with AI
		if (TestAIGroup.CollideWithAI(MainPlayer.GetCollisionXBoundOne(), MainPlayer.GetCollisionYBoundOne()))
			MainPlayer.MovementCollidingBoundOne();
		if (TestAIGroup.CollideWithAI(MainPlayer.GetCollisionXBoundTwo(), MainPlayer.GetCollisionYBoundTwo()))
			MainPlayer.MovementCollidingBoundTwo();
		// Hit the AI
		TestAIGroup.HitAI(MainPlayer.GetWeaponHitBoxXBoundOne(), MainPlayer.GetWeaponHitBoxYBoundOne(), MainPlayer.GetWeaponDamage());
		TestAIGroup.HitAI(MainPlayer.GetWeaponHitBoxXBoundTwo(), MainPlayer.GetWeaponHitBoxYBoundTwo(), MainPlayer.GetWeaponDamage());

		if (Dungeon.Get_Map()->CheckMapCollision(Vec2f(MainPlayer.GetCollisionXBoundOne(), MainPlayer.GetCollisionYBoundOne())))
			MainPlayer.MovementCollidingBoundOne();
		if (Dungeon.Get_Map()->CheckMapCollision(Vec2f(MainPlayer.GetCollisionXBoundTwo(), MainPlayer.GetCollisionYBoundTwo())))
			MainPlayer.MovementCollidingBoundTwo();


		//Code Dealing with drawing to the screen goes within this if statement
		if (al_is_event_queue_empty(Event_Queue))
		{
			Dungeon.Draw();
			MainPlayer.DrawPlayer();
			//Terrain.draw();
			TestAIGroup.DrawAll();  // Draw all AI (magenta squares). Their generated paths will also be drawn (small magenta circles).
			
			//Draw display last
			MainDisplay.Draw();
		}
	}

	
	//Game Ending--------------------------------------

	return 0;
}
//Created by:		Ryan Nolan-Hieb, Scott Backer, and James Beller

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
#include "GUILayer.h"


int main(void)
{
	//Initialize allegro--------------------------------
	if (InitAllegro())
	{
		return -1;
	}

	srand(time(NULL));

	//variables-----------------------------------------
	const int FPS = 60;//how many frames the game should run at
	bool StartScreen = true; //start screen while loop bool
	bool QuitGame = false;
	int NumberOfAI = 30;

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

	ALLEGRO_BITMAP *AIImage = al_load_bitmap("AI_Sprite.png");
	ALLEGRO_BITMAP *StartImage = al_load_bitmap("Start_Screen.jpg");
	ALLEGRO_BITMAP *GameOverImage = al_load_bitmap("GameOver_Screen.jpg");

	Player MainPlayer(Event_Queue);
	Camera MainCamera(Event_Queue, MainDisplay.Get_ScreenWidth(), MainDisplay.Get_ScreenHeight());
	AI_Group TestAIGroup(Event_Queue);  // Instance of an AI_Group
	GUILayer MainGUILayer(MainDisplay.Get_ScreenWidth(), MainDisplay.Get_ScreenHeight());

	DungeonGenerator Dungeon(Event_Queue, &MainPlayer);
	Dungeon.GenerateDungeon(MainDisplay);

	MainPlayer.SetXPosition(Dungeon.GetStartPosition().x());
	MainPlayer.SetYPosition(Dungeon.GetStartPosition().y());

	TestAIGroup.RandomSetup(NumberOfAI, Dungeon);  // Generates AI with random attributes in the group. Their spawn points will also be set randomly.
	
	al_register_event_source(Event_Queue, al_get_timer_event_source(Timer));
	al_register_event_source(Event_Queue, al_get_keyboard_event_source());
	al_register_event_source(Event_Queue, al_get_mouse_event_source());
	

	al_start_timer(Timer);
	
	//Main game loop------------------------------------
	while (!QuitGame)
	{
		while (StartScreen)
		{
			//allegro event and queue
			ALLEGRO_EVENT ev;
			al_wait_for_event(Event_Queue, &ev);

			//if space pressed end start screen
			if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
			{
				StartScreen = false;
				al_add_timer_count(Timer, al_get_timer_count(Timer) * -1);
			}

			if (al_is_event_queue_empty(Event_Queue))
			{
				//draw title image
				al_draw_bitmap(StartImage, 0, 0, NULL);

				//Draw display last
				MainDisplay.Draw();
			}
		}

		ALLEGRO_EVENT ev;
		al_wait_for_event(Event_Queue, &ev);
		
		Dungeon.Event_Handler(ev);
		TestAIGroup.EventHandler(ev);
		TestAIGroup.ProcessAll(ev, MainPlayer);  // Process each AI in the group
		MainPlayer.EventHandler(ev, MainCamera.GetMouseXWorldCoordinate(), MainCamera.GetMouseYWorldCoordinate());
		MainCamera.EventHandler(ev, MainPlayer.GetXPosition(), MainPlayer.GetYPosition());
		
		if (MainDisplay.Event_Handler(ev))
			QuitGame = true;

		//Current Dungeon complete move on to the next one
		if (ev.type == DUNGEON_COMPLETE_EVENT)
		{
			al_stop_timer(Timer); //Pause the timer while all the new level loads
			Dungeon.GenerateDungeon(MainDisplay);
			TestAIGroup.RandomSetup(30, Dungeon);
			MainPlayer.SetXPosition(Dungeon.GetStartPosition().x());
			MainPlayer.SetYPosition(Dungeon.GetStartPosition().y());
			MainPlayer.ScaleGameUp(Dungeon.Get_DungeonLevel());
			al_start_timer(Timer); //resume the timer after the new level loads
		}

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


		if (MainPlayer.IsDead())
		{
			MainCamera.ResetTranslate();

			bool GameOverScreen = true; //bool for game over screen sequence
			int PlayerFinalScore = MainPlayer.GetFinalTimedScore(Timer);
			
			while (GameOverScreen)
			{
				//allegro event and queue
				ALLEGRO_EVENT ev;
				al_wait_for_event(Event_Queue, &ev);

				//if space pressed end start screen
				if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
				{
					StartScreen = true;
					GameOverScreen = false;
				}
				else if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
				{
					GameOverScreen = false;
				}

				if (al_is_event_queue_empty(Event_Queue))
				{
					//draw title image
					al_draw_bitmap(GameOverImage, 0, 0, NULL);
					MainGUILayer.DrawFinalScoreScreen(PlayerFinalScore);
					//Draw display last
					MainDisplay.Draw();
				}
			}

			
			MainPlayer.ResetPlayer();
			Dungeon.Set_DungeonLevel(1);
			Dungeon.GenerateDungeon(MainDisplay);
			TestAIGroup.RandomSetup(NumberOfAI += Random(2,5), Dungeon);
			MainPlayer.SetXPosition(Dungeon.GetStartPosition().x());
			MainPlayer.SetYPosition(Dungeon.GetStartPosition().y());
			MainPlayer.ScaleGameUp(Dungeon.Get_DungeonLevel());
			al_add_timer_count(Timer, al_get_timer_count(Timer) * -1);
		}

		//Code Dealing with drawing to the screen goes within this if statement
		if (al_is_event_queue_empty(Event_Queue))
		{
			Dungeon.Draw(1); //Draw the bottom layers of the dungeon
			MainPlayer.DrawPlayer(); //Draw the player
			TestAIGroup.DrawAll();  // Draw all AI.
			Dungeon.Draw(0); //Draw the top layers of the dungeon

			MainGUILayer.DrawPlayerInformation(MainCamera, MainPlayer.GetCurrentLevel(), MainPlayer.GetCurrentHealth());
			MainGUILayer.DrawScoreInformation(MainCamera, MainPlayer.GetCurrentScore());
			MainGUILayer.DrawGameTimeInformation(MainCamera, Timer);

			//Draw display last
			MainDisplay.Draw();
		}
	}

	
	//Game Ending--------------------------------------

	return 0;
}
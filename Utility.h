//Created by:		Ryan Nolan-Hieb

#ifndef __UTILITY__
#define __UTILITY__

#include <map>
#include <string>

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>

//Utility/Global useful functions/enums

//All of the user custom events that we have created
enum CUSTOM_EVENT_ID 
{ 
	PROJECTILE_EVENT = 1025, 
	PLAYERPOSITION_EVENT, 
	FOODPICKUP_EVENT, 
	SPEEDPICKUP_EVENT, 
	STRENGTHPICKUP_EVENT,
	AI_KILLED_EVENT,
	TERRAINTILE_TRIGGER_EVENT,
	MELEEATTACK_EVENT,
	PICKUP_EVENT,
	SPAWN_BOSS_EVENT,
	BOSS_KILLED_EVENT,
	DUNGEON_COMPLETE_EVENT,
	ENTER_DUNGEON
};


//used for generating a random number between two values. Should probably be moved to a different file but again, I'm lazy :P.
int Random(int RandomAmount1, int RandomAmount2);

//initialize allegro
int InitAllegro();

#endif
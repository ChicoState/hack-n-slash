//Scott Backer

//FoodPickup Class H: Food Pickup class that is a Pickup Object that gives health to the player.

#include <allegro5\allegro.h>

#ifndef FOODPICKUP_H
#define FOODPICKUP_H

#include "PickupObject.h"

class FoodPickup : public PickupObject
{
public:
	FoodPickup(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent, int SpawnXPosition, int SpawnYPosition); //Constructor for the FoodPickup object class
	void DeletePickup(); //Calls delete on the pickup for it to execute its ending statements
	void EventHandler(ALLEGRO_EVENT& InputAlEvent); //Event handler for the FoodPickup class
	void delete_callback(ALLEGRO_USER_EVENT *InputAlEvent);

private:
	ALLEGRO_EVENT_SOURCE m_FoodPickupEventSource; //event source for emitting player position
};

#endif
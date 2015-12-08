//Scott Backer

//FoodPickup Class H: Food Pickup class that is a Pickup Object that gives health to the player.

#include <allegro5\allegro.h>

#ifndef FOODPICKUP_H
#define FOODPICKUP_H

#include "PickupObject.h"

class FoodPickup : public PickupObject
{
public:
	FoodPickup(ALLEGRO_EVENT_QUEUE* InputEventQueue, int SpawnXPosition, int SpawnYPosition); //Constructor for the FoodPickup object class
	void EventHandler(ALLEGRO_EVENT&); //Event handler for the FoodPickup class
	void delete_callback(ALLEGRO_USER_EVENT *InputAlEvent);

private:
};

#endif
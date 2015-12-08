//Scott Backer

//StrengthPickup Class H: Strength Pickup class that is a Pickup Object that gives health to the player.

#include <allegro5\allegro.h>

#ifndef StrengthPICKUP_H
#define StrengthPICKUP_H

#include "PickupObject.h"

class StrengthPickup : public PickupObject
{
public:
	StrengthPickup(ALLEGRO_EVENT_QUEUE* InputEventQueue, int SpawnXPosition, int SpawnYPosition); //Constructor for the StrengthPickup object class
	void EventHandler(ALLEGRO_EVENT&); //Event handler for the StrengthPickup class

private:
};

#endif
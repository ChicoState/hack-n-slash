//Scott Backer

//SpeedPickup Class H: Speed Pickup class that is a Pickup Object that gives health to the player.

#include <allegro5\allegro.h>

#ifndef SpeedPICKUP_H
#define SpeedPICKUP_H

#include "PickupObject.h"

class SpeedPickup : public PickupObject
{
public:
	SpeedPickup(ALLEGRO_EVENT_QUEUE* InputEventQueue, int SpawnXPosition, int SpawnYPosition); //Constructor for the SpeedPickup object class
	void EventHandler(ALLEGRO_EVENT&); //Event handler for the SpeedPickup class

private:
};

#endif
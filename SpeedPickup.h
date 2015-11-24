//Scott Backer

//SpeedPickup Class H: Speed Pickup class that is a Pickup Object that gives health to the player.

#include <allegro5\allegro.h>

#ifndef SpeedPICKUP_H
#define SpeedPICKUP_H

#include "PickupObject.h"

class SpeedPickup : public PickupObject
{
public:
	SpeedPickup(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent, int SpawnXPosition, int SpawnYPosition); //Constructor for the SpeedPickup object class
	void DeletePickup(); //Calls delete on the pickup for it to execute its ending statements
	void EventHandler(ALLEGRO_EVENT& InputAlEvent); //Event handler for the SpeedPickup class

private:
	ALLEGRO_EVENT_SOURCE m_SpeedPickupEventSource; //event source for emitting player position
};

#endif
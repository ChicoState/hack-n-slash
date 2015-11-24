//Scott Backer

//StrengthPickup Class H: Strength Pickup class that is a Pickup Object that gives health to the player.

#include <allegro5\allegro.h>

#ifndef StrengthPICKUP_H
#define StrengthPICKUP_H

#include "PickupObject.h"

class StrengthPickup : public PickupObject
{
public:
	StrengthPickup(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent, int SpawnXPosition, int SpawnYPosition); //Constructor for the StrengthPickup object class
	void DeletePickup(); //Calls delete on the pickup for it to execute its ending statements
	void EventHandler(ALLEGRO_EVENT& InputAlEvent); //Event handler for the StrengthPickup class

private:
	ALLEGRO_EVENT_SOURCE m_StrengthPickupEventSource; //event source for emitting player position
};

#endif
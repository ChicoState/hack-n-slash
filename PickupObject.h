//Scott Backer

//PickupObject Class H: Pickup Object class allows objects to be droppped and picked up in game.

#ifndef PICKUPOBJECT_H
#define PICKUPOBJECT_H

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

#include "PickupObjectTile.h"
#include "Utility.h"

class PickupObject
{
public:
	PickupObject(ALLEGRO_BITMAP *SpriteImage, ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent, int SpawnXPosition, int SpawnYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool Animated, bool Continuous, bool Looped, int AnimFPS); //Constructor for the PickupObject class
	void EventHandler(); //Event handler for the PickupObject class
	void Draw(); //Draws the PickupObject

	int GetXBound(); //Gets and returns the X Bound
	int GetYBound(); //Gets and returns the Y Bound 
	int GetXPosition(); //!Gets and returns the X position of the pickup object
	int GetYPosition(); //!Gets and returns the y position of the pickup object

protected:
	ALLEGRO_EVENT_QUEUE *m_EventQueue; //The event queue for the pickupobject class
	ALLEGRO_EVENT m_AlEvent; //the event variable for the pickup object class

	int m_XBound; //the x bound
	int m_YBound; //the y bound
	int m_XPosition; //x position of the object
	int m_YPosition; //y position of the object

private:
	PickupObjectTile m_SpriteImage;

};

#endif
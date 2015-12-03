//Scott Backer

//PickupObject Class CPP: Pickup Object class allows objects to be droppped and picked up in game.

#include "PickupObject.h"

//Constructor for the PickupObject class
PickupObject::PickupObject(ALLEGRO_BITMAP *SpriteImage, ALLEGRO_EVENT_QUEUE* InputEventQueue, int SpawnXPosition, int SpawnYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool Animated, bool Continuous, bool Looped, int AnimFPS)
	: m_SpriteImage(SpriteImage, SpawnXPosition, SpawnYPosition, FrameWidth, FrameHeight, Collidable, Animated, Continuous, Looped, AnimFPS)
{
	//set member variables
	m_EventQueue = InputEventQueue;
	m_XBound = FrameWidth;
	m_YBound = FrameHeight;
	m_XPosition = SpawnXPosition;
	m_YPosition = SpawnYPosition;
	m_IsDead = false;

	al_init_user_event_source(&m_PickupEvent);
	al_register_event_source(m_EventQueue, &m_PickupEvent);
}

//Draws the PickupObject
void PickupObject::Draw()
{
	m_SpriteImage.Draw(m_XPosition, m_YPosition);
}

//Calls delete on the pickup for it to execute its ending statements
void PickupObject::DeletePickup()
{
	al_unregister_event_source(m_EventQueue, &m_PickupEvent);
}

//Returns whether or not the pickup item has been picked up and should be deleted
bool PickupObject::IsPickupDead()
{
	//Return true if the pickup object is dead and already picked up else false
	if(m_IsDead)
	{
		return true;
	}

	else
	{
		return false;
	}
}

//!Gets and returns the PickupObject X Bound
//Out - 
//		int - player x bound
int PickupObject::GetXBound()
{
	return m_XBound;
}

//!Gets and returns the PickupObject Y Bound
//Out - 
//		int - player y bound
int PickupObject::GetYBound()
{
	return m_YBound;
}

//!Gets and returns the X position of the pickup object
//Out - 
//		int - the current x position of the pickup object
int PickupObject::GetXPosition()
{
	return m_XPosition;
}

//!Gets and returns the Y position of the pickup object
//Out - 
//		int - the current y position of the pickup object
int PickupObject::GetYPosition()
{
	return m_YPosition;
}
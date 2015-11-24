//Scott Backer

//SpeedPickup Class CPP: Speed Pickup class that is a Pickup Object that gives speed to the player.

#include "SpeedPickup.h"

//Constructor for SpeedPickup class
SpeedPickup::SpeedPickup(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent, int SpawnXPosition, int SpawnYPosition) 
	: PickupObject(al_load_bitmap("Food_Sprite.jpg"), InputEventQueue, InputAlEvent, SpawnXPosition, SpawnYPosition, 28, 28, true, false, false, false, 1)
{
	al_init_user_event_source(&m_SpeedPickupEventSource);
	al_register_event_source(m_EventQueue, &m_SpeedPickupEventSource);
}

//Calls delete on the pickup for it to execute its ending statements
void SpeedPickup::DeletePickup()
{
	al_unregister_event_source(m_EventQueue, &m_SpeedPickupEventSource);
}

//Event handler for the SpeedPickup class
void SpeedPickup::EventHandler(ALLEGRO_EVENT& InputAlEvent)
{
	//check the update player position event
	if(InputAlEvent.type == PLAYERPOSITION_EVENT)
	{
		//get the player position data
		int TempPlayerXPosition = InputAlEvent.user.data1;
		int TempPlayerYPosition = InputAlEvent.user.data2;

		//check to see if the player is inside this objects bounds
		if(TempPlayerXPosition > m_XPosition - m_XBound &&
			TempPlayerXPosition < m_XPosition + m_XBound &&
			TempPlayerYPosition > m_YPosition - m_YBound &&
			TempPlayerYPosition < m_YPosition + m_YBound)
		{
			//emit the event source that the Speed collided with the player
			m_AlEvent.user.type = CUSTOM_EVENT_ID(SPEEDPICKUP_EVENT);
			al_emit_user_event(&m_SpeedPickupEventSource, &m_AlEvent, NULL);
			m_IsDead = true;
		}
	}
}
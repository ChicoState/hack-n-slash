//Scott Backer

//FoodPickup Class CPP: Food Pickup class that is a Pickup Object that gives health to the player.

#include "FoodPickup.h"

//Constructor for FoodPickup class
FoodPickup::FoodPickup(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent, int SpawnXPosition, int SpawnYPosition) 
	: PickupObject(al_load_bitmap("Pickup_Food.png"), InputEventQueue, InputAlEvent, SpawnXPosition, SpawnYPosition, 44, 44, true, false, false, false, 1)
{
	al_init_user_event_source(&m_FoodPickupEventSource);
	al_register_event_source(m_EventQueue, &m_FoodPickupEventSource);
}

//Calls delete on the pickup for it to execute its ending statements
void FoodPickup::DeletePickup()
{
	al_unregister_event_source(m_EventQueue, &m_FoodPickupEventSource);
}

//Event handler for the FoodPickup class
void FoodPickup::EventHandler(ALLEGRO_EVENT& InputAlEvent)
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
			//emit the event source that the food collided with the player
			m_AlEvent.user.type = CUSTOM_EVENT_ID(FOODPICKUP_EVENT);
			al_emit_user_event(&m_FoodPickupEventSource, &m_AlEvent, NULL);
			m_IsDead = true;
		}
	}
}
//Scott Backer

//StrengthPickup Class CPP: Strength Pickup class that is a Pickup Object that gives Strength to the player.

#include "StrengthPickup.h"

//Constructor for StrengthPickup class
StrengthPickup::StrengthPickup(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent, int SpawnXPosition, int SpawnYPosition) 
	: PickupObject(al_load_bitmap("Food_Sprite.jpg"), InputEventQueue, InputAlEvent, SpawnXPosition, SpawnYPosition, 28, 28, true, false, false, false, 1)
{
	al_init_user_event_source(&m_StrengthPickupEventSource);
	al_register_event_source(m_EventQueue, &m_StrengthPickupEventSource);
}

//Calls delete on the pickup for it to execute its ending statements
void StrengthPickup::DeletePickup()
{
	al_unregister_event_source(m_EventQueue, &m_StrengthPickupEventSource);
}

//Event handler for the StrengthPickup class
void StrengthPickup::EventHandler(ALLEGRO_EVENT& InputAlEvent)
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
			//emit the event source that the Strength collided with the player
			m_AlEvent.user.type = CUSTOM_EVENT_ID(STRENGTHPICKUP_EVENT);
			al_emit_user_event(&m_StrengthPickupEventSource, &m_AlEvent, NULL);
			m_IsDead = true;
		}
	}
}
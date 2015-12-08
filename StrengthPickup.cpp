//Scott Backer

//StrengthPickup Class CPP: Strength Pickup class that is a Pickup Object that gives Strength to the player.

#include "StrengthPickup.h"

//Constructor for StrengthPickup class
StrengthPickup::StrengthPickup(ALLEGRO_EVENT_QUEUE* InputEventQueue, int SpawnXPosition, int SpawnYPosition) 
	: PickupObject(al_load_bitmap("Pickup_Power.png"), InputEventQueue, SpawnXPosition, SpawnYPosition, 44, 44, true, false, false, false, 1)
{
}

//Calls delete on the pickup for it to execute its ending statements

//Event handler for the StrengthPickup class
void StrengthPickup::EventHandler(ALLEGRO_EVENT& EV)
{
	//check the update player position event
	if(EV.type == PLAYERPOSITION_EVENT)
	{
		//get the player position data
		int TempPlayerXPosition = EV.user.data1;
		int TempPlayerYPosition = EV.user.data2;

		//check to see if the player is inside this objects bounds
		if(TempPlayerXPosition > m_XPosition - m_XBound &&
			TempPlayerXPosition < m_XPosition + m_XBound &&
			TempPlayerYPosition > m_YPosition - m_YBound &&
			TempPlayerYPosition < m_YPosition + m_YBound)
		{
			//emit the event source that the Strength collided with the player
			EV.user.type = CUSTOM_EVENT_ID(PICKUP_EVENT);
			EV.user.data1 = (intptr_t)STR;
			EV.user.data2 = (intptr_t)this;
			al_emit_user_event(&m_PickupEvent, &EV, NULL);
			m_IsDead = true;
		}
	}
}
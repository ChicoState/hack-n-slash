//Scott Backer

//FoodPickup Class CPP: Food Pickup class that is a Pickup Object that gives health to the player.

#include "FoodPickup.h"

//Constructor for FoodPickup class
FoodPickup::FoodPickup(ALLEGRO_EVENT_QUEUE* InputEventQueue, int SpawnXPosition, int SpawnYPosition) 
	: PickupObject(al_load_bitmap("Pickup_Food.png"), InputEventQueue, SpawnXPosition, SpawnYPosition, 44, 44, true, false, false, false, 1)
{
}

//Event handler for the FoodPickup class
void FoodPickup::EventHandler(ALLEGRO_EVENT& EV)
{
	//check the update player position event
	if (EV.type == PLAYERPOSITION_EVENT)
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
			//emit the event source that the food collided with the player
			EV.user.type = CUSTOM_EVENT_ID(PICKUP_EVENT);
			EV.user.data1 = (intptr_t)FOOD;
			EV.user.data2 = (intptr_t)this;
			al_emit_user_event(&m_PickupEvent, &EV, NULL);
			m_IsDead = true;
		}
	}
}
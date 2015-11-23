//Scott Backer

//FoodPickup Class CPP: Food Pickup class that is a Pickup Object that gives health to the player.

#include "FoodPickup.h"
#include "Player.h"

FoodPickup::FoodPickup(ALLEGRO_BITMAP *SpriteImage, ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent, int SpawnXPosition, int SpawnYPosition) 
	: PickupObject(al_load_bitmap("Food_Sprite.jpg"), InputEventQueue, InputAlEvent, SpawnXPosition, SpawnYPosition, 28, 28, true, false, false, false, 1)
{
	al_init_user_event_source(&m_FoodPickupEventSource);
	al_register_event_source(m_EventQueue, &m_FoodPickupEventSource);
}

void FoodPickup::DeleteFoodPickup()
{
	al_unregister_event_source(m_EventQueue, &m_FoodPickupEventSource);
	delete this;
	
}

void FoodPickup::EventHandler(ALLEGRO_EVENT& InputAlEvent)
{
	if(this != NULL && InputAlEvent.type == PLAYERPOSITION_EVENT)
	{
		int TempPlayerXPosition = InputAlEvent.user.data1;
		int TempPlayerYPosition = InputAlEvent.user.data2;

		if(TempPlayerXPosition > m_XPosition - m_XBound &&
			TempPlayerXPosition < m_XPosition + m_XBound &&
			TempPlayerYPosition > m_YPosition - m_YBound &&
			TempPlayerYPosition < m_YPosition + m_YBound)
		{
			//emit the event source that the food collided with the player
			m_AlEvent.user.type = CUSTOM_EVENT_ID(FOODPICKUP_EVENT);
			m_AlEvent.user.data1 = (intptr_t)this;
			al_emit_user_event(&m_FoodPickupEventSource, &m_AlEvent, NULL);
			//DeleteFoodPickup();
		}
	}
}
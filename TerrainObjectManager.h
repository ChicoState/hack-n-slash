#ifndef __TERRAIN_OBJECT_MANAGER__
#define __TERRAIN_OBJECT_MANAGER__


#include <allegro5\allegro.h>
#include <vector>
#include "Sprite.h"

#include "PickupObject.h"
#include "SpeedPickup.h"
#include "StrengthPickup.h"
#include "FoodPickup.h"
#include "Vec2.h"
#include "Utility.h"

class TerrainObject_Manager
{
private:
	int m_DropChance;
	
	std::vector<PickupObject*> m_Objects;

	//ALLEGRO_EVENT_SOURCE; //event source for emitting player position
	ALLEGRO_EVENT_QUEUE *m_EventQueue;

public:
	TerrainObject_Manager(ALLEGRO_EVENT_QUEUE *EventQueue) :
		m_EventQueue(EventQueue), m_DropChance(0)
	{
	}
	~TerrainObject_Manager()
	{
		for (int i = 0; i < m_Objects.size(); i++)
		{
			delete m_Objects[i];
		}
	}
	
	//Given a Position (in pixels), and a base chance to spawn. Has a random chance to spawn a pickup object 
	void SpawnObjectRandom(Vec2i, int);
	//Given a Position (in pixels), and a type. spawns a pickup object of that type
	void SpawnObject(Vec2i, PICKUP_TYPES);
	
	//Useful function for reseting the Object Manager
	void ResetObjectManager();

	void Draw();
	int Event_Handler(ALLEGRO_EVENT&);

};


#endif

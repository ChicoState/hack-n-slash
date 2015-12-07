//Created by:	Ryan Nolan-Hieb

#include "TerrainObjectManager.h"


void TerrainObject_Manager::SpawnObject(Vec2i Pos, PICKUP_TYPES Type)
{
	if (Type == FOOD) //creates a new food (aka health) pickup
	{
		FoodPickup *Temp = new FoodPickup(m_EventQueue, Pos.x(), Pos.y());
		m_Objects.push_back(Temp);
	}
	else if (Type == STR) //creates a new strength pickup
	{
		StrengthPickup *Temp = new StrengthPickup(m_EventQueue, Pos.x(), Pos.y());
		m_Objects.push_back(Temp);
	}
	else //creates a new strength pickup
	{
		SpeedPickup *Temp = new SpeedPickup(m_EventQueue, Pos.x(), Pos.y());
		m_Objects.push_back(Temp);
	}
}
void TerrainObject_Manager::Draw()
{
	for (unsigned int i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->Draw(); // draw each object
	}
}
int TerrainObject_Manager::Event_Handler(ALLEGRO_EVENT& EV)
{
	for (unsigned int i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects[i]->IsPickupDead())
		{
			//if the pickup is dead get rid of it
			delete m_Objects[i];
			m_Objects.erase(m_Objects.begin() + i);
			continue;
		}

		//run the events for each object
		m_Objects[i]->EventHandler(EV);
	}

	return 0;
}

void TerrainObject_Manager::SpawnObjectRandom(Vec2i Pos, int ChanceToSpawn)
{
	//by default there will be a 60% for food and a 20% change for strength and speed power ups to drop
	int ChanceForFood = 60;
	int ChanceForSpeed = 20;

	//has to pass the base check first though to see if a pickup will even spawn
	if (Random(0, 100) > ChanceToSpawn)
		return;

	int RandNum = Random(0, 100);

	if (RandNum <= ChanceForFood)
	{
		SpawnObject(Vec2i(Pos.x(), Pos.y()), FOOD);
	}
	else if (RandNum < ChanceForFood + ChanceForSpeed)
	{
		SpawnObject(Vec2i(Pos.x(), Pos.y()), SPEED);
	}
	else
	{
		SpawnObject(Vec2i(Pos.x(), Pos.y()), STR);
	}
}

void TerrainObject_Manager::ResetObjectManager()
{
	for (unsigned int i = 0; i < m_Objects.size(); i++)
	{
		delete m_Objects[i];
	}

	m_Objects.clear();
}
#include "TerrainObjectManager.h"


void TerrainObject_Manager::SpawnObject(Vec2i Pos, PICKUP_TYPES Type)
{
	if (Type == FOOD)
	{
		FoodPickup *Temp = new FoodPickup(m_EventQueue, Pos.x(), Pos.y());
		m_Objects.push_back(Temp);
	}
	else if (Type == STR)
	{
		StrengthPickup *Temp = new StrengthPickup(m_EventQueue, Pos.x(), Pos.y());
		m_Objects.push_back(Temp);
	}
	else
	{
		SpeedPickup *Temp = new SpeedPickup(m_EventQueue, Pos.x(), Pos.y());
		m_Objects.push_back(Temp);
	}
}
void TerrainObject_Manager::Draw()
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->Draw();
	}
}
int TerrainObject_Manager::Event_Handler(ALLEGRO_EVENT& EV)
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects[i]->IsPickupDead())
		{
			delete m_Objects[i];
			m_Objects.erase(m_Objects.begin() + i);
			continue;
		}

		m_Objects[i]->EventHandler(EV);
	}

	return 0;
}

void TerrainObject_Manager::SpawnObjectRandom(Vec2i Pos, int ChanceToSpawn)
{
	int ChanceForFood = 60;
	int ChanceForSTR = 20;
	int ChanceForSpeed = 20;

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
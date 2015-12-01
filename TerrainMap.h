#ifndef __TERRAIN_MAP__
#define __TERRAIN_MAP__

#include <vector>

#include "TerrainLayer.h"
#include "Projectile.h"
#include "Utility.h"
#include "Player.h"
#include "PickupObject.h"
#include "TerrainObjectManager.h"

//TerrainMap is responsible for controlling the entire Terrain.
class TerrainMap
{
private:
	std::vector<TerrainLayer*> m_Map;
	std::vector<std::vector<TerrainTile*>> m_InfoLayer;
	int m_MapSizeX; //The number of tiles wide the Map is
	int m_MapSizeY; //The number of tiles tall the Map is
	int m_TileSize; //The size of a single tile (terrain tiles are assumed to be square)

	TerrainObject_Manager m_ObjectManager;

	Player *m_MainPlayer;

	ALLEGRO_EVENT_QUEUE *m_EventQueue;

public:
	TerrainMap(ALLEGRO_EVENT_QUEUE *EventQueue, TerrainLayer* BaseLayer, Player *MainPlayer) : m_MainPlayer(MainPlayer), m_EventQueue(EventQueue),
		m_ObjectManager(EventQueue)
	{
		m_Map.push_back(BaseLayer);
		m_TileSize = BaseLayer->Get_TileSize();
		m_MapSizeX = BaseLayer->Get_LayerWidth() / m_TileSize;
		m_MapSizeY = BaseLayer->Get_LayerHeight() / m_TileSize;
		
		TerrainMap::InitInfoLayer();
	}
	~TerrainMap()
	{
		for (unsigned int i = 0; i < m_Map.size(); i++)
		{
			delete m_Map[i];
		}
	}

	
	TerrainLayer* Get_Layer(int index){ return m_Map[index]; }

	void InitInfoLayer();
	void AddLayer(TerrainLayer*, bool);
	void UpdateInfoLayer(int);


	bool CheckMapCollision(Vec2f);

	void Event_Handler(ALLEGRO_EVENT&);
	void Draw(bool);

private:
	void ClearFog(Vec2i, int);
	void CreatePickupObjects(Vec2i);

};

#endif
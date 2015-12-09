//Created by:	 Ryan Nolan-Hieb

#ifndef __TERRAIN_MAP__
#define __TERRAIN_MAP__

#include <vector>

#include "TerrainLayer.h"
#include "Projectile.h"
#include "Utility.h"
#include "Player.h"
#include "PickupObject.h"
#include "TerrainObjectManager.h"

//TerrainMap is responsible for controlling the entire graphical Terrain.
class TerrainMap
{
private:
	std::vector<TerrainLayer*> m_Map; //The map is comprised of multiple terrain layers

	std::vector<std::vector<TerrainTile*>> m_InfoLayer; //the info layer basically acts 
														//as easy interface for determining if a specific area has tiles 
														//that are collideable or triggerable
	int m_MapSizeX; //The number of tiles wide the Map is
	int m_MapSizeY; //The number of tiles tall the Map is
	int m_TileSize; //The size of a single tile (terrain tiles are assumed to be square)

	TerrainObject_Manager m_ObjectManager; //The manager for controlling all of the pickup objects that get spawned from the terrain

	Player *m_MainPlayer; //pointer to the player

	ALLEGRO_EVENT_QUEUE *m_EventQueue; //pointer to the main event queue

public:
	TerrainMap(ALLEGRO_EVENT_QUEUE *EventQueue, TerrainLayer* BaseLayer, Player *MainPlayer) : m_MainPlayer(MainPlayer), m_EventQueue(EventQueue),
		m_ObjectManager(EventQueue)
	{
		m_Map.push_back(BaseLayer);
		m_TileSize = BaseLayer->Get_TileSize(); //get the size of a single tile (terrains are expected to be made of square tiles)
		m_MapSizeX = BaseLayer->Get_LayerWidth() / m_TileSize;  //calculating the width of the terrain
		m_MapSizeY = BaseLayer->Get_LayerHeight() / m_TileSize; //calculating the height of the terrain
		
		TerrainMap::InitInfoLayer(); //populates the info layer with information from the base layer
	}
	~TerrainMap()
	{
		//clean up all of the layers since those get dynamically created
		for (unsigned int i = 0; i < m_Map.size(); i++)
		{
			delete m_Map[i];
		}
	}

	//Basic Getters and Setters
	
	TerrainLayer* Get_Layer(int index){ return m_Map[index]; }


	//Core functions

	void AddLayer(TerrainLayer*, bool); //adds a layer and then if true, will add the information to the info layer
	void UpdateInfoLayer(int); //update the info layer at the given layer index


	bool CheckMapCollision(Vec2f); //checks to see if the given position collides with anything on the terrain

	void Event_Handler(ALLEGRO_EVENT&); //handles events
	void Draw(bool); //draws the map

private:
	void InitInfoLayer(); //populates the info layer with information
	void ClearFog(Vec2i, int); //recursive algorithm for removing all fog from a room once the player enters it
	void CreatePickupObjects(Vec2i); //tells the object manager to spawn an object
	void CheckProjectiles(std::vector<Projectile*>&); //checks projectiles to see if they're hitting any parts of the dungeon
	int GetLayerFromType(Vec2i, TRIGGER); //Given a position index and a trigger type, returns the layer that tile is on
};

#endif
#ifndef __TERRAIN_MAP__
#define __TERRAIN_MAP__

#include <vector>

#include "TerrainLayer.h"


class TerrainMap
{
private:
	std::vector<TerrainLayer*> m_Map;
	std::vector<std::vector<TerrainTile*>> m_InfoLayer;
	int m_MapSizeX;
	int m_MapSizeY;
	int m_TileSize;

public:
	TerrainMap()
	{}
	TerrainMap(TerrainLayer* BaseLayer)
	{
		m_Map.push_back(BaseLayer);
		m_TileSize = BaseLayer->Get_TileSize();
		m_MapSizeX = BaseLayer->Get_LayerWidth() / m_TileSize;
		m_MapSizeY = BaseLayer->Get_LayerHeight() / m_TileSize;
		
		TerrainMap::InitInfoLayer();
	}
	~TerrainMap()
	{
		for (int i = 0; i < m_Map.size(); i++)
		{
			m_Map[i]->~TerrainLayer();
			delete m_Map[i];
		}
	}

	
	TerrainLayer* Get_Layer(int index){ return m_Map[index]; }

	void InitInfoLayer();
	void AddLayer(TerrainLayer*, bool);
	void UpdateInfoLayer(int);
	
	bool CheckMapCollision(Vec2f);

	virtual void Draw();

};

#endif
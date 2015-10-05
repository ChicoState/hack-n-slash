#ifndef __TERRAIN_LAYER__
#define __TERRAIN_LAYER__

#include <vector>

#include "TerrainTile.h"
#include "Display.h"

class TerrainLayer
{
private:
	std::vector<std::vector<TerrainTile>> m_Layer;
	
	std::vector<TerrainTile> m_AnimatedTiles;

	ALLEGRO_BITMAP *m_LayerImage;

	int m_LayerWidth;
	int m_LayerHeight;

public:
	TerrainLayer() : m_LayerImage(NULL), m_LayerWidth(0), m_LayerHeight(0)
	{}
	TerrainLayer(std::vector<std::vector<TerrainTile>> Layer) : m_Layer(Layer), m_LayerImage(NULL)
	{
		m_LayerWidth = m_Layer[0][0].Get_TileWidth() * m_Layer.size();
		m_LayerHeight = m_Layer[0][0].Get_TileHeight() * m_Layer[0].size();			
	}
	~TerrainLayer()
	{
		al_destroy_bitmap(m_LayerImage);
	}
	
	void Draw();
	void CreateBitmap(Display&);
};

#endif
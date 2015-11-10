#include "TerrainLayer.h"

void TerrainLayer::Draw()
{
	al_draw_bitmap(m_LayerImage, 0, 0, 0);
	
	for (int i = 0; i < m_AnimatedTiles.size(); i++)
	{
		m_AnimatedTiles[i].Draw();
	}
}

void TerrainLayer::Event_Handler(ALLEGRO_EVENT &EV)
{
	for (int i = 0; i < m_AnimatedTiles.size(); i++)
	{
		m_AnimatedTiles[i].Event_Handler(EV);
	}
}


void TerrainLayer::CreateBitmap(Display &MainDisplay)
{
	m_LayerImage = al_create_bitmap(m_LayerWidth, m_LayerHeight);

	
	al_set_target_bitmap(m_LayerImage);

	for (int x = 0; x < m_Layer.size(); x++)
	{
		for (int y = 0; y < m_Layer[0].size(); y++)
		{
			if (m_Layer[x][y].Get_IsAnimated())
			{
				m_AnimatedTiles.push_back(m_Layer[x][y]);
			}
			else
			{
				m_Layer[x][y].Draw();
			}
		}
	}

	MainDisplay.SetDisplayAsTarget();
	
}
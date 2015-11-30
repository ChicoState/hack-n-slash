#include "TerrainLayer.h"

void TerrainLayer::Draw()
{
	al_draw_bitmap(m_LayerImage, 0, 0, 0);
	
	for (int i = 0; i < m_AnimatedTiles.size(); i++)
	{
		m_AnimatedTiles[i]->Draw();
	}
}

void TerrainLayer::Event_Handler(ALLEGRO_EVENT &EV)
{
	for (int i = 0; i < m_AnimatedTiles.size(); i++)
	{
		m_AnimatedTiles[i]->Event_Handler(EV);
	}
	for (int i = 0; i < m_TriggerTiles.size(); i++)
	{
		m_TriggerTiles[i]->Event_Handler(EV);

		if (m_TriggerTiles[i]->Get_TriggerType() == TR_RETURN && EV.type == BOSS_KILLED_EVENT)
		{
			CreateBitmap(NULL);
		}
	}
}

void TerrainLayer::CreateBitmap(Display *MainDisplay)
{
	if (m_LayerImage == NULL)
	{
		m_MainDisplay = MainDisplay;
		m_LayerImage = al_create_bitmap(m_LayerWidth, m_LayerHeight);
	}

	m_AnimatedTiles.clear();
	m_TriggerTiles.clear();
	
	al_set_target_bitmap(m_LayerImage);
	
	al_clear_to_color(al_map_rgba(0, 0, 0, 0));
	
	for (int x = 0; x < m_Layer.size(); x++)
	{
		for (int y = 0; y < m_Layer[0].size(); y++)
		{
			if (m_Layer[x][y].Get_Retired())
				continue;

			if (m_Layer[x][y].Get_IsAnimated())
			{
				m_AnimatedTiles.push_back(&m_Layer[x][y]);
			}
			else if (m_Layer[x][y].Get_TriggerTile())
			{
				m_TriggerTiles.push_back(&m_Layer[x][y]);
				m_Layer[x][y].Draw();
			}
			else
			{
				m_Layer[x][y].Draw();
			}
		}
	}

	m_MainDisplay->SetDisplayAsTarget();
	
}
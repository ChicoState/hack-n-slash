//Created by:	Ryan Nolan-Hieb

#include "TerrainLayer.h"

void TerrainLayer::Draw()
{
	//draw the bitmap version of the layer
	al_draw_bitmap(m_LayerImage, 0, 0, 0);
	
	//for any animated tiles that are left, draw those
	for (unsigned int i = 0; i < m_AnimatedTiles.size(); i++)
	{
		m_AnimatedTiles[i]->Draw();
	}
}

void TerrainLayer::Event_Handler(ALLEGRO_EVENT &EV)
{
	//update animated tiles
	for (unsigned int i = 0; i < m_AnimatedTiles.size(); i++)
	{
		m_AnimatedTiles[i]->Event_Handler(EV);
	}
	//run the event handles on all of the trigger tiles
	for (unsigned int i = 0; i < m_TriggerTiles.size(); i++)
	{
		m_TriggerTiles[i]->Event_Handler(EV);

		//the layer that contains the boss portal needs to update it's bitmap when the boss dies
		if (m_TriggerTiles[i]->Get_TriggerType() == TR_RETURN && EV.type == BOSS_KILLED_EVENT)
		{
			CreateBitmap(NULL);
		}
	}
}

void TerrainLayer::CreateBitmap(Display *MainDisplay)
{
	//Rather than creating a whole new Createbitmap function that works at the beginning when the display gets passed in,
	//I have this check here that will basically check to see if the user is calling this function for the first time or not
	//If he is then it runs though some setup stuff
	if (m_LayerImage == NULL && MainDisplay != NULL)
	{
		m_MainDisplay = MainDisplay;
		m_LayerImage = al_create_bitmap(m_LayerWidth, m_LayerHeight);
	}

	//clear everything
	m_AnimatedTiles.clear();
	m_TriggerTiles.clear();
	
	//set the bitmap to draw to to be the LayerImage
	al_set_target_bitmap(m_LayerImage);
	
	//clear the map
	al_clear_to_color(al_map_rgba(0, 0, 0, 0));
	
	for (unsigned int x = 0; x < m_Layer.size(); x++)
	{
		for (unsigned int y = 0; y < m_Layer[0].size(); y++)
		{
			//don't draw retired tiles
			if (m_Layer[x][y].Get_Retired())
				continue;

			if (m_Layer[x][y].Get_IsAnimated()) //add animated tiles to the animated tiles array, don't draw them to the map
			{
				m_AnimatedTiles.push_back(&m_Layer[x][y]);
			}
			else if (m_Layer[x][y].Get_TriggerTile()) //add trigger tiles to the trigger tiles array, then draw them to the map
			{
				m_TriggerTiles.push_back(&m_Layer[x][y]);
				m_Layer[x][y].Draw();
			}
			else //simply draw all the basic tiles to the bitmap
			{
				m_Layer[x][y].Draw();
			}
		}
	}

	//bitmap created, set the display back to being the default bitmap to draw to
	m_MainDisplay->SetDisplayAsTarget();
}

void TerrainLayer::SetTerrainImage()
{
	m_LayerImage = al_load_bitmap("terrain1.jpg");
}
//Created by:		Ryan Nolan-Hieb
//Date updated:		9/25/15
//Last update by:	N/A
//Reason for update:N/A

#include "TerrainTile.h"

#include <allegro5\allegro_primitives.h>

void TerrainTile::Draw()
{
	if (m_Image != NULL)
	{
		Sprite::Draw();
	}
	else
	{
		if (m_TileType == Wall)
		{
			al_draw_filled_rectangle(m_PosX, m_PosY, m_PosX + m_FrameWidth, m_PosY + m_FrameHeight, al_map_rgb(255, 0, 0));
		}
		else if (m_TileType == Floor)
		{
			al_draw_filled_rectangle(m_PosX, m_PosY, m_PosX + m_FrameWidth, m_PosY + m_FrameHeight, al_map_rgb(0, 255, 0));
		}
		else if (m_TileType == Door)
		{
			al_draw_filled_rectangle(m_PosX, m_PosY, m_PosX + m_FrameWidth, m_PosY + m_FrameHeight, al_map_rgb(0, 0, 255));
		}
	}
}

int TerrainTile::Event_Handler(ALLEGRO_EVENT &EV)
{
	return Sprite::Event_Handler(EV);
}